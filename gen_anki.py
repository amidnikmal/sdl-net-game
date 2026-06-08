#!/usr/bin/env python3
from __future__ import annotations

import argparse
import hashlib
import html
import json
import re
import sqlite3
import tempfile
import time
import zipfile
from pathlib import Path


ROOT_DECK_NAME = "Программирование::C::Сетевая игра::Микроатомы"
ACTIVE_DECK_NAME = f"{ROOT_DECK_NAME}::Active"
BACKLOG_DECK_NAME = f"{ROOT_DECK_NAME}::Backlog"

DEFAULT_TEMPLATE = "collection-2026-06-08@19-54-08.colpkg"
DEFAULT_OUTPUT = "sdl-net-game-code-review.apkg"

ROOT_DECK_ID = 1780938235000
ACTIVE_DECK_ID = 1780938235001
BACKLOG_DECK_ID = 1780938235002

NOTE_ID_BASE = 1780939000000
CARD_ID_BASE = 1781940000000
FIELD_SEPARATOR = "\x1f"

ACTIVE_DESCRIPTION = (
    "5 активных микроатомов по C и сетям для этого проекта. "
    "Идея: не энциклопедия, а маленький рабочий словарь различений."
)
BACKLOG_DESCRIPTION = (
    "Отложенные микроатомы по C и сетям. Хранить, но не тащить всё сразу в активное повторение."
)
ROOT_DESCRIPTION = (
    "Общая колода микроатомов по C и сетям, собранная по мотивам разбора проекта sdl-net-game."
)

ACTIVE_CARDS: list[tuple[str, str]] = [
    (
        "Stack vs heap: в чём разница и кто отвечает за освобождение памяти?",
        """Stack живёт до конца текущего scope: `free()` не нужен и недопустим.
Heap выделяется через `malloc/calloc/realloc`: нужен `free()`.

Главное различение: освобождает тот, кто владеет буфером по контракту API.""",
    ),
    (
        "`recv()` vs `recvfrom()`: когда что использовать в UDP-коде?",
        """`recv()` читает данные, когда peer уже известен.
`recvfrom()` читает данные и одновременно возвращает адрес отправителя.

Обычно:
- клиент на connected socket может использовать `recv()`
- UDP-серверу чаще нужен `recvfrom()`""",
    ),
    (
        "Неблокирующий сокет: `recv() == -1` — это всегда fatal error?",
        """Нет.
При `O_NONBLOCK` это часто `EAGAIN` / `EWOULDBLOCK`: данных сейчас нет.

Правильное действие: не парсить буфер и выйти из итерации.
Это надо отличать от реальной I/O ошибки.""",
    ),
    (
        "`INADDR_ANY`: где это правильно, а где нет?",
        """Правильно: серверный `bind()`, чтобы слушать на всех интерфейсах.
Неправильно: использовать как адрес назначения клиента.

Клиенту нужен конкретный peer address, а не `0.0.0.0`.""",
    ),
    (
        "Почему две макроконстанты с одинаковым числом нельзя подменять друг другом?",
        """Потому что у константы есть смысл, а не только значение.

Если `USERNAME_MAX_SIZE == PACKET_HEADER_SIZE == 64`, это не делает их взаимозаменяемыми.
Константу выбирают по семантике, иначе код становится хрупким.""",
    ),
]

BACKLOG_CARDS: list[tuple[str, str]] = [
    (
        "`htons/htonl` vs `ntohs/ntohl`: зачем вообще эти функции?",
        """Они переводят числа между host byte order и network byte order.

`htons/htonl`: host -> network
`ntohs/ntohl`: network -> host

Нужны, чтобы одинаково трактовать байты на разных машинах.""",
    ),
    (
        "Почему `socklen_t size` перед `recvfrom()` надо переустанавливать на каждой итерации?",
        """Потому что это value-result аргумент:
- на входе: максимальный размер адресной структуры
- на выходе: фактический размер записанного адреса

Если не переустановить, следующий вызов может получить уже уменьшенный размер.""",
    ),
    (
        "`strncpy(dst, src, n)`: что означает `n` на самом деле?",
        """`n` — это размер целевого буфера, а не длина строки-источника.

Ошибка мышления:
`strncpy(dst, src, strlen(src) + 1)`

Это не защита от переполнения. Защита начинается от знания размера `dst`.""",
    ),
    (
        "Почему `return -1;` из функции с типом `uint8_t` — плохая идея?",
        """Потому что `uint8_t` беззнаковый.
`-1` преобразуется в `255`.

Код может «работать случайно», но смысл становится мутным.
Лучше возвращать явную константу вроде `UNKNOWN_PACKET_ID = 255`.""",
    ),
    (
        "Почему transport/helper-функция не должна сама делать `free(buffer)`?",
        """Потому что helper часто не знает, чей это буфер:
- heap
- stack
- static memory
- чужая область

`free()` должен вызываться там, где определён ownership.
Иначе легко получить invalid free.""",
    ),
    (
        "Почему пакет по сети лучше собирать как `header + payload`, а не слепо кастовать `struct`?",
        """Потому что layout структуры зависит от:
- padding
- alignment
- порядка байт

Явная сериализация даёт стабильный протокол.
`struct cast` удобен, но хрупок для сетевого формата.""",
    ),
    (
        "Blocking vs non-blocking socket: в чём разница для цикла программы?",
        """Blocking socket ждёт событие и останавливает текущий поток.
Non-blocking socket сразу возвращает управление, даже если данных нет.

Выбор влияет на архитектуру цикла:
- blocking проще
- non-blocking требует явной обработки `EAGAIN/EWOULDBLOCK`""",
    ),
    (
        "Почему `exit()` внутри helper/library-функции — антипаттерн?",
        """Потому что helper не должен сам решать судьбу всего процесса.

Правильнее:
- вернуть код ошибки
- дать вызывающему коду решить, логировать ли, ретраить ли, завершать ли программу""",
    ),
    (
        "`sizeof(ptr)` vs `sizeof(*ptr)`: что нужно для `malloc()`?",
        """Аллоцировать нужно размер того объекта, на который указывает указатель.

Обычно:
`p = malloc(sizeof(*p));`

Так меньше риск ошибиться уровнем косвенности или типом.""",
    ),
    (
        "Почему плохо держать одну и ту же координату в двух несинхронных полях?",
        """Потому что появляется два источника правды.

Если `player.x_coord` обновили, а `player.rect.x` забыли, код начинает расходиться сам с собой.
Либо одно каноническое поле, либо синхронное обновление везде.""",
    ),
]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate a compact Anki deck with active and backlog micro-atoms for sdl-net-game."
    )
    parser.add_argument(
        "--template",
        default=DEFAULT_TEMPLATE,
        help="Path to a template collection (.colpkg or collection.anki2).",
    )
    parser.add_argument(
        "--output",
        default=DEFAULT_OUTPUT,
        help="Output .apkg path.",
    )
    return parser.parse_args()


def load_template_collection(template_path: Path) -> bytes:
    if not template_path.exists():
        raise FileNotFoundError(f"Template not found: {template_path}")

    if template_path.suffix == ".colpkg":
        with zipfile.ZipFile(template_path) as archive:
            return archive.read("collection.anki2")

    return template_path.read_bytes()


def find_basic_model(models: dict[str, dict]) -> tuple[int, dict]:
    for key, model in models.items():
        field_names = [field["name"] for field in model.get("flds", [])]
        if model.get("name") == "Basic" and field_names[:2] == ["Front", "Back"]:
            return int(key), model

    raise RuntimeError("Could not find a Basic note type in the template collection.")


def guid_for(front: str) -> str:
    return hashlib.sha1(front.encode("utf-8")).hexdigest()[:10]


def checksum_for(front: str) -> int:
    text = html.unescape(re.sub(r"<[^>]+>", "", front))
    digest = hashlib.sha1(text.encode("utf-8")).hexdigest()
    return int(digest[:8], 16)


def deck_payload(deck_id: int, name: str, mod_ts: int, desc: str, collapsed: bool) -> dict:
    return {
        "id": deck_id,
        "mod": mod_ts,
        "name": name,
        "usn": 0,
        "lrnToday": [0, 0],
        "revToday": [0, 0],
        "newToday": [0, 0],
        "timeToday": [0, 0],
        "collapsed": collapsed,
        "browserCollapsed": collapsed,
        "desc": desc,
        "dyn": 0,
        "conf": 1,
        "extendNew": 0,
        "extendRev": 0,
        "reviewLimit": None,
        "newLimit": None,
        "reviewLimitToday": None,
        "newLimitToday": None,
        "desiredRetention": None,
    }


def iter_cards() -> list[tuple[int, str, str, str]]:
    cards: list[tuple[int, str, str, str]] = []
    for front, back in ACTIVE_CARDS:
        cards.append((ACTIVE_DECK_ID, front, back, "active"))
    for front, back in BACKLOG_CARDS:
        cards.append((BACKLOG_DECK_ID, front, back, "backlog"))
    return cards


def rebuild_collection(db_path: Path) -> None:
    db = sqlite3.connect(db_path)
    db.row_factory = sqlite3.Row
    cur = db.cursor()

    cur.execute("DELETE FROM cards")
    cur.execute("DELETE FROM notes")
    cur.execute("DELETE FROM revlog")
    cur.execute("DELETE FROM graves")

    col = cur.execute("SELECT * FROM col").fetchone()
    conf = json.loads(col["conf"])
    models = json.loads(col["models"])
    decks = json.loads(col["decks"])
    dconf = json.loads(col["dconf"])

    model_id, basic_model = find_basic_model(models)
    mod_ts = int(time.time())
    scm_ts = int(time.time() * 1000)
    cards = iter_cards()

    for key in list(conf):
        if key.startswith("_deck_") or key.startswith("_nt_"):
            del conf[key]

    conf["curDeck"] = ACTIVE_DECK_ID
    conf["activeDecks"] = [ACTIVE_DECK_ID]
    conf["curModel"] = model_id
    conf["nextPos"] = len(cards) + 1
    conf[f"_deck_{ACTIVE_DECK_ID}_lastNotetype"] = model_id
    conf[f"_nt_{model_id}_lastDeck"] = ACTIVE_DECK_ID

    decks = {
        "1": decks["1"],
        str(ROOT_DECK_ID): deck_payload(ROOT_DECK_ID, ROOT_DECK_NAME, mod_ts, ROOT_DESCRIPTION, False),
        str(ACTIVE_DECK_ID): deck_payload(
            ACTIVE_DECK_ID,
            ACTIVE_DECK_NAME,
            mod_ts,
            ACTIVE_DESCRIPTION,
            False,
        ),
        str(BACKLOG_DECK_ID): deck_payload(
            BACKLOG_DECK_ID,
            BACKLOG_DECK_NAME,
            mod_ts,
            BACKLOG_DESCRIPTION,
            True,
        ),
    }
    models = {str(model_id): basic_model}

    cur.execute(
        """
        UPDATE col
        SET mod = ?, scm = ?, usn = 0, ls = 0, conf = ?, models = ?, decks = ?, dconf = ?, tags = '{}'
        WHERE id = 1
        """,
        (
            mod_ts,
            scm_ts,
            json.dumps(conf, ensure_ascii=False, separators=(",", ":")),
            json.dumps(models, ensure_ascii=False, separators=(",", ":")),
            json.dumps(decks, ensure_ascii=False, separators=(",", ":")),
            json.dumps(dconf, ensure_ascii=False, separators=(",", ":")),
        ),
    )

    for idx, (deck_id, front, back, bucket) in enumerate(cards, start=1):
        note_id = NOTE_ID_BASE + idx
        card_id = CARD_ID_BASE + idx
        fields = FIELD_SEPARATOR.join((front, back))
        tags = f" sdl-net-game micro-atom {bucket} "

        cur.execute(
            """
            INSERT INTO notes (id, guid, mid, mod, usn, tags, flds, sfld, csum, flags, data)
            VALUES (?, ?, ?, ?, 0, ?, ?, ?, ?, 0, '')
            """,
            (
                note_id,
                guid_for(front),
                model_id,
                mod_ts,
                tags,
                fields,
                front,
                checksum_for(front),
            ),
        )
        cur.execute(
            """
            INSERT INTO cards (
                id, nid, did, ord, mod, usn, type, queue, due, ivl,
                factor, reps, lapses, left, odue, odid, flags, data
            )
            VALUES (?, ?, ?, 0, ?, 0, 0, 0, ?, 0, 2500, 0, 0, 0, 0, 0, 0, '{}')
            """,
            (
                card_id,
                note_id,
                deck_id,
                mod_ts,
                idx,
            ),
        )

    db.commit()
    cur.execute("PRAGMA optimize")
    integrity = cur.execute("PRAGMA integrity_check").fetchone()[0]
    db.close()

    if integrity != "ok":
        raise RuntimeError(f"SQLite integrity check failed: {integrity}")


def build_package(template_bytes: bytes, output_path: Path) -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        db_path = Path(tmpdir) / "collection.anki2"
        db_path.write_bytes(template_bytes)
        rebuild_collection(db_path)
        db_bytes = db_path.read_bytes()

        with zipfile.ZipFile(output_path, "w", compression=zipfile.ZIP_DEFLATED) as archive:
            archive.writestr("collection.anki21", db_bytes)
            archive.writestr("collection.anki2", db_bytes)
            archive.writestr("media", "{}")


def validate_package(output_path: Path) -> None:
    with zipfile.ZipFile(output_path) as archive:
        names = sorted(archive.namelist())
        expected = ["collection.anki2", "collection.anki21", "media"]
        if names != expected:
            raise RuntimeError(f"Unexpected archive members: {names}")
        if "meta" in names:
            raise RuntimeError("The generated .apkg must not include meta.")

        media = archive.read("media").decode("utf-8")
        if media != "{}":
            raise RuntimeError(f"Unexpected media manifest: {media}")

        db_bytes = archive.read("collection.anki21")

    with tempfile.TemporaryDirectory() as tmpdir:
        db_path = Path(tmpdir) / "validate.anki21"
        db_path.write_bytes(db_bytes)
        db = sqlite3.connect(db_path)
        cur = db.cursor()
        integrity = cur.execute("PRAGMA integrity_check").fetchone()[0]
        note_count = cur.execute("SELECT COUNT(*) FROM notes").fetchone()[0]
        card_count = cur.execute("SELECT COUNT(*) FROM cards").fetchone()[0]
        decks = cur.execute("SELECT decks FROM col").fetchone()[0]
        db.close()

    expected_count = len(ACTIVE_CARDS) + len(BACKLOG_CARDS)
    if integrity != "ok":
        raise RuntimeError(f"Generated package failed integrity check: {integrity}")
    if note_count != expected_count or card_count != expected_count:
        raise RuntimeError(
            f"Unexpected card counts: notes={note_count}, cards={card_count}, expected={expected_count}"
        )
    if ACTIVE_DECK_NAME not in decks or BACKLOG_DECK_NAME not in decks:
        raise RuntimeError("Expected deck names are missing from generated collection.")


def main() -> int:
    args = parse_args()
    template_path = Path(args.template)
    output_path = Path(args.output)

    template_bytes = load_template_collection(template_path)
    build_package(template_bytes, output_path)
    validate_package(output_path)

    total = len(ACTIVE_CARDS) + len(BACKLOG_CARDS)
    print(
        f"Generated {output_path} with {len(ACTIVE_CARDS)} active cards and "
        f"{len(BACKLOG_CARDS)} backlog cards ({total} total)."
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

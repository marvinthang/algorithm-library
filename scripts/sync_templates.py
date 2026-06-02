#!/usr/bin/env python3
from __future__ import annotations

from collections import OrderedDict
from pathlib import Path
import argparse
import json
import shutil
import subprocess
import tempfile
import zipfile


ROOT = Path(__file__).resolve().parents[1]
LIB = ROOT / "lib"
INCLUDE_PATH = str(LIB)

SNIPPETS_FILE = ROOT / "marvinthang.code-snippets"
PROFILE_FILES = [
    ROOT / "marvinthang.code-profile",
    ROOT / "marvinthang_linux.code-profile",
    ROOT / "marvinthang_v2.code-profile",
]
SUBLIME_RAR = ROOT / "Sublime Text.rar"
SUBLIME_ZIP = ROOT / "Sublime Text.zip"

SOURCE_MAP = OrderedDict(
    [
        ("START", "start.h"),
        ("AHO_CORASICK_ARR", "string/aho_corasick_arr.h"),
        ("AHO_CORASICK_PTR", "string/aho_corasick_ptr.h"),
        ("SPARSE_TABLE", "data-structures/sparse_table.h"),
        ("SEGTREE", "data-structures/segtree.h"),
        ("RANDOM", "miscellaneous/random.h"),
        ("ROUND", "miscellaneous/round.h"),
        ("PARTIALLY_PERSISTENT_DSU", "data-structures/partially_persistent_disjoint_set.h"),
        ("ORDERED_SET", "miscellaneous/ordered_set.h"),
        ("OFFSET_VECTOR", "data-structures/offset_vector.h"),
        ("RABIN_MILLER", "math/rabin_miller.h"),
        ("NUMBER_THEORETIC_TRANSFORM", "math/number_theoretic_transform.h"),
        ("FAST_FOURIER_TRANSFORM", "math/fast_fourier_transform.h"),
        ("MODINT", "math/modint.h"),
        ("MO_ALGORITHM", "data-structures/mo_algorithm.h"),
        ("MIN_COST_FLOW", "graph/max-flow/min_cost_flow.h"),
        ("MAXMIN", "miscellaneous/maxmin.h"),
        ("MATRIX", "math/matrix.h"),
        ("LINE_CONTAINER", "dynamic-programming/line_container.h"),
        ("LICHAO_TREE", "dynamic-programming/lichao_tree.h"),
        ("LCA_RMQ", "graph/dfs-tree/lca_rmq.h"),
        ("LCA_BL", "graph/dfs-tree/lca_bl.h"),
        ("LAZY_SEGTREE", "data-structures/lazy_segtree.h"),
        ("KMP", "string/kmp.h"),
        ("GRAPH", "graph/graph.h"),
        ("HASHING", "string/hashing.h"),
        ("GRADER", "grader/grader.h"),
        ("GEOMETRY", "geometry.h"),
        ("GENERATOR", "grader/generator.h"),
        ("GAUSSIAN_ELIMINATION", "math/gaussian_elimination.h"),
        ("FENWICK_TREE", "data-structures/fenwick_tree.h"),
        ("FENWICK_TREE_RANGE", "data-structures/fenwick_tree_range.h"),
        ("FASTIO_KACTL", "miscellaneous/fastio_kactl.h"),
        ("EDMONDS_KARP", "graph/max-flow/edmonds_karp.h"),
        ("DISJOINT_SPARSE_TABLE", "data-structures/disjoint_sparse_table.h"),
        ("DISJOINT_SET_UNION", "data-structures/disjoint_set_union.h"),
        ("CONVEX_HULL_TRICK", "dynamic-programming/convex_hull_trick.h"),
        ("COMPRESS", "miscellaneous/compress.h"),
        ("BUMP_ALLOCATOR", "miscellaneous/bump_allocator.h"),
        ("BRIDGE_ARTICULATION", "graph/dfs-tree/bridge_articulation.h"),
        ("BITSET_TRICK", "miscellaneous/bitset_trick.h"),
        ("BIGINT", "math/bigint.h"),
        ("BENCHMARK", "grader/benchmark.h"),
        ("Z_FUNCTION", "string/z_function.h"),
        ("WAVELET_TREE", "data-structures/wavelet_tree.h"),
        ("TWO_SAT", "graph/two_sat.h"),
        ("SUFFIX_ARRAY", "string/suffix_array.h"),
        ("SUBSET_CONVOLUTION", "math/subset_convolution.h"),
        ("STRONGLY_CONNECTED_COMPONENT", "graph/dfs-tree/strongly_connected_component.h"),
        ("MANACHER", "string/manacher.h"),
        ("MAX_FLOW", "graph/max-flow/max_flow.h"),
        ("BLOSSOM", "graph/blossom.h"),
        ("GCD", "math/gcd.h"),
        ("XOR_BASIS_ONLINE", "data-structures/xor_basis_online.h"),
    ]
)

PREFIX_ALIASES = {
    "GAUSSIAN_ELIMINATION": ["GAUSSIAN_ELIMINATION", "GAUSS_ELIMINATION"],
    "KMP": ["KMP", "KNUTH_MORRIS_PRATT"],
}

OLD_ARCHIVE = "_archive" + "_"
OLD_WIN_INCLUDE = "-I" + "D:/code/" + OLD_ARCHIVE + "/lib"
OLD_LINUX_INCLUDE = "-I" + "/run/media/marvinthang/marvinthang/code/" + OLD_ARCHIVE + "/lib"
OLD_STACK = "--" + "stack=100000000"
OLD_STACK_BIG = "-stack," + "1000000000"
OLD_STACK_COMMA = "-stack," + "100000000"
OLD_STACK_256M = "--" + "stack=268435456"

PROFILE_REPLACEMENTS = {
    OLD_WIN_INCLUDE: f"-I{INCLUDE_PATH}",
    OLD_LINUX_INCLUDE: f"-I{INCLUDE_PATH}",
    f' "-Wl,{OLD_STACK}"': "",
    f" -Wl,{OLD_STACK}": "",
    f' "-Wl,{OLD_STACK_COMMA}"': "",
    f" -Wl,{OLD_STACK_COMMA}": "",
}

SUBLIME_BUILD_REPLACEMENTS = {
    OLD_WIN_INCLUDE: f"-I{INCLUDE_PATH}",
    OLD_LINUX_INCLUDE: f"-I{INCLUDE_PATH}",
    f" -Wl,{OLD_STACK_BIG}": "",
    f" -Wl,{OLD_STACK_COMMA}": "",
    f" -Wl,{OLD_STACK_256M}": "",
    f" -Wl,{OLD_STACK}": "",
}


def read_source_lines(name: str, mode: str) -> list[str]:
    return (LIB / SOURCE_MAP[name]).read_text().splitlines()


def build_vscode_snippets() -> OrderedDict:
    snippets = OrderedDict()
    for name in SOURCE_MAP:
        snippets[name] = OrderedDict(
            [
                ("scope", "cpp"),
                ("prefix", PREFIX_ALIASES.get(name, name)),
                ("body", read_source_lines(name, "vscode")),
                ("description", name),
            ]
        )
    return snippets


def write_vscode_snippets(snippets: OrderedDict) -> None:
    SNIPPETS_FILE.write_text(json.dumps(snippets, indent=4, ensure_ascii=False) + "\n")


def update_profiles(snippets: OrderedDict) -> None:
    embedded = json.dumps(snippets, indent=4, ensure_ascii=False)
    for path in PROFILE_FILES:
        data = json.loads(path.read_text(), object_pairs_hook=OrderedDict)
        data["snippets"] = embedded
        outer_settings = json.loads(data["settings"], object_pairs_hook=OrderedDict)
        settings = outer_settings.get("settings", "")
        for old, new in PROFILE_REPLACEMENTS.items():
            settings = settings.replace(old, new)
        outer_settings["settings"] = settings
        data["settings"] = json.dumps(outer_settings, ensure_ascii=False)
        path.write_text(json.dumps(data, indent=4, ensure_ascii=False) + "\n")


def snippet_xml(name: str) -> str:
    body = "\n".join(read_source_lines(name, "sublime"))
    return (
        "<snippet>\n"
        "    <content><![CDATA[\n"
        f"{body}\n"
        "]]></content>\n"
        f"    <tabTrigger>{name}</tabTrigger>\n"
        "    <scope>source.c++</scope>\n"
        "</snippet>\n"
    )


def prepare_sublime_tree(tmp: Path) -> Path:
    if SUBLIME_ZIP.exists():
        with zipfile.ZipFile(SUBLIME_ZIP, "r") as archive:
            archive.extractall(tmp)
    elif SUBLIME_RAR.exists() and shutil.which("unrar"):
        subprocess.run(["unrar", "x", "-idq", str(SUBLIME_RAR), str(tmp)], check=True)
    else:
        (tmp / "Sublime Text" / "Packages" / "User").mkdir(parents=True)
    user_dir = tmp / "Sublime Text" / "Packages" / "User"
    user_dir.mkdir(parents=True, exist_ok=True)
    return user_dir


def update_sublime_files(user_dir: Path) -> None:
    for name in SOURCE_MAP:
        (user_dir / f"{name}.sublime-snippet").write_text(snippet_xml(name))
    build_file = user_dir / "build-cpp.sublime-build"
    if build_file.exists():
        text = build_file.read_text()
        for old, new in SUBLIME_BUILD_REPLACEMENTS.items():
            text = text.replace(old, new)
        build_file.write_text(text)
    cppbuilder = user_dir / "CppBuilder.sublime-settings"
    if cppbuilder.exists():
        text = cppbuilder.read_text()
        text = text.replace(
            '"include_dir": [\n    ],',
            f'"include_dir": [\n        "{INCLUDE_PATH}"\n    ],',
        )
        text = text.replace('"std=gnu++11"', '"std=c++20"')
        cppbuilder.write_text(text)


def write_sublime_zip_from_base(base: Path) -> None:
    with zipfile.ZipFile(SUBLIME_ZIP, "w", compression=zipfile.ZIP_DEFLATED) as archive:
        for path in sorted(base.rglob("*")):
            if path.is_file():
                archive.write(path, Path("Sublime Text") / path.relative_to(base))


def write_sublime_rar(tmp: Path) -> bool:
    if not shutil.which("rar"):
        return False
    tmp_rar = ROOT / "Sublime Text.synced.rar"
    if tmp_rar.exists():
        tmp_rar.unlink()
    subprocess.run(["rar", "a", "-r", "-ma5", str(tmp_rar), "Sublime Text"], cwd=tmp, check=True)
    tmp_rar.replace(SUBLIME_RAR)
    return True


def update_extracted_sublime(base: Path) -> None:
    user_dir = base / "Packages" / "User"
    user_dir.mkdir(parents=True, exist_ok=True)
    update_sublime_files(user_dir)
    write_sublime_zip_from_base(base)


def update_sublime(write_rar: bool) -> bool:
    with tempfile.TemporaryDirectory(prefix="sublime-sync-") as name:
        tmp = Path(name)
        user_dir = prepare_sublime_tree(tmp)
        update_sublime_files(user_dir)
        write_sublime_zip_from_base(tmp / "Sublime Text")
        return write_rar and write_sublime_rar(tmp)


def check_current(snippets: OrderedDict) -> list[str]:
    problems: list[str] = []
    if json.loads(SNIPPETS_FILE.read_text(), object_pairs_hook=OrderedDict) != snippets:
        problems.append(f"{SNIPPETS_FILE.name} is not synced")
    for path in PROFILE_FILES:
        data = json.loads(path.read_text(), object_pairs_hook=OrderedDict)
        if json.loads(data["snippets"], object_pairs_hook=OrderedDict) != snippets:
            problems.append(f"{path.name} snippets are not synced")
        settings = json.loads(data["settings"])["settings"]
        for old in PROFILE_REPLACEMENTS:
            if old in settings:
                problems.append(f"{path.name} still contains {old}")
    if not SUBLIME_ZIP.exists():
        problems.append(f"{SUBLIME_ZIP.name} does not exist")
    else:
        with zipfile.ZipFile(SUBLIME_ZIP, "r") as archive:
            names = set(archive.namelist())
            for name in SOURCE_MAP:
                zip_name = f"Sublime Text/Packages/User/{name}.sublime-snippet"
                if zip_name not in names:
                    problems.append(f"{SUBLIME_ZIP.name} missing {zip_name}")
                    continue
                text = archive.read(zip_name).decode("utf-8", errors="replace")
                if text != snippet_xml(name):
                    problems.append(f"{zip_name} is not synced")
            build_name = "Sublime Text/Packages/User/build-cpp.sublime-build"
            if build_name in names:
                build_text = archive.read(build_name).decode("utf-8", errors="replace")
                for old in SUBLIME_BUILD_REPLACEMENTS:
                    if old in build_text:
                        problems.append(f"{build_name} still contains {old}")
    return problems


def main() -> int:
    parser = argparse.ArgumentParser(description="Sync editor templates from lib/.")
    parser.add_argument("--check", action="store_true", help="check sync without writing files")
    parser.add_argument("--write-rar", action="store_true", help="also write a local Sublime Text.rar when rar is installed")
    parser.add_argument("--sublime-dir", type=Path, help="update an extracted Sublime Text folder in place")
    args = parser.parse_args()

    snippets = build_vscode_snippets()
    if args.check:
        problems = check_current(snippets)
        if problems:
            for problem in problems:
                print(problem)
            return 1
        print("VS Code snippets, profiles, and Sublime Text.zip are synced")
        return 0

    write_vscode_snippets(snippets)
    update_profiles(snippets)
    if args.sublime_dir:
        update_extracted_sublime(args.sublime_dir)
        rar_updated = False
        print(f"synced VS Code snippets, profiles, {SUBLIME_ZIP.name}, and {args.sublime_dir}")
    else:
        rar_updated = update_sublime(args.write_rar)
        print("synced VS Code snippets, profiles, and Sublime Text.zip")
    if args.write_rar and rar_updated:
        print("synced local Sublime Text.rar")
    elif args.write_rar:
        print("Sublime Text.rar was not rewritten because no rar binary is available")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

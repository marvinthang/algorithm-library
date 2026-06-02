# Algorithm Library

Competitive-programming templates, snippets, and editor profiles kept in sync with the source files under `lib/`.

## Layout

- `lib/`: C++ template source files.
- `marvinthang.code-snippets`: VS Code snippets generated from the library templates.
- `marvinthang*.code-profile`: VS Code profiles with the same snippets embedded.
- `Sublime Text.zip`: Sublime Text profile mirror generated from the current library templates.

## Notes

The C++ profiles compile with `-std=c++20` and include this library through:

```sh
-I/home/marvinthang/projects/algorithm-library/lib
```

The repository is already initialized on branch `main` with the `origin` remote set to `https://github.com/marvinthang/algorithm-library.git`.

## Syncing Templates

Regenerate editor snippets and profiles from `lib/` with:

```sh
python3 scripts/sync_templates.py
```

Update an extracted Sublime Text folder in place with:

```sh
python3 scripts/sync_templates.py --sublime-dir "Sublime Text"
```

Check whether generated files are already synced with:

```sh
python3 scripts/sync_templates.py --check
```

The script always updates `Sublime Text.zip`. `Sublime Text.rar` is ignored because this environment cannot rewrite RAR archives and the old archive contained stale snippets. If you install a `rar` command and want a local RAR copy, run:

```sh
python3 scripts/sync_templates.py --write-rar
```

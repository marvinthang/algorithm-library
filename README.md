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

Check whether generated files are already synced with:

```sh
python3 scripts/sync_templates.py --check
```

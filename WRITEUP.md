cat > ~/lab1/WRITEUP.md <<'EOF'
Shandy Salam
# Lab1 Write-up: Who Owns the Lexeme?
A lexeme is the exact text stored in a token, for example `route` or `->`. In `token_make`, the code calls `malloc` to copy that text onto the heap. That means every token gets its own small heap string. When I say “owns,” I mean the token handler must later release that string. If no one releases it, then the memory is leaked.

In `main.c`, tokens are added to the list using `list_push_back`. The deal is simple: once you push, the list takes over. After `list_push_back` returns `true`, the list is the one that must manage the lexeme. The caller should not free it.

There are two spots where the lexeme is released. In `list_destroy`, the code walks through each node, frees the token that is inside, and then frees the node itself. In `list_remove_first`, it detaches the first matching node, then frees the token inside it, and then frees the node. In both cases, you free the token before freeing the node. If you free the node first, you may not be able to access fields like its `next` pointer anymore.

If `malloc` fails while `push_back` is trying to build a new node, the list cannot add it. Still, the token exists, and it still holds ownership of its lexeme. On that failure path, `push_back` calls `token_free(&token)` before it returns `false`. That keeps the lexeme from leaking.

I ran `make valgrind`, and it reports 0 errors and 0 leaks.
EOF


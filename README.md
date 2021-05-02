# nix_4coder_commands
Nick Mayfield's 4Coder Commands (Some personal customizations for 4Coder that could be of some use to others)

## Rectangle Edit
`#include "nix_rectangle_edit.cpp"`

This command queries the user for a string, which can be empty. The right-most column is not included in the rectangle. When undoing, the current implementation requires an undo for every line that was edited.

How to:

* **Kill:**
  * place cursor and mark in different columns
  * enter an empty string

* **Replace:**
  * place cursor and mark in different columns
  * enter a string into query

* **Insert:**
  * place cursor and mark in the same column on different lines
  * enter a string into query

Future improvements:
* Make rectangle edits work with lines that don't extend into the specified rectangle
* Edit all at once so only one 'undo' is required

## Reload Changed Buffers
`#include "nix_reload_changed_buffers.cpp"`

This command reloads all opened buffers that have Unloaded changes (marked with '!' in lister).

## Kill Buffer
`#include "nix_get_buffers_opened_first.cpp"`
`#include "nix_kill_buffer.cpp"`

This one is only a slight change to interactive_kill_buffer. The only difference is the current buffer is listed first, which works better for my brain.


/////////////////////////////////////////////////////////////////////
// 01110000 01110011 01100100 01110010 01101110 01100100 01101101 //
///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// nick@psdrndm.com

CUSTOM_UI_COMMAND_SIG(nix_reload_changed_buffers)
CUSTOM_DOC("Reload buffers with unloaded changes.")
{
    Scratch_Block scratch(app);
    for (Buffer_ID buffer = get_buffer_next(app, 0, Access_Always);
         buffer != 0;
         buffer = get_buffer_next(app, buffer, Access_Always)){
        
        Dirty_State dirty = buffer_get_dirty_state(app, buffer);
        if (dirty == DirtyState_UnloadedChanges) {
            buffer_reopen(app, buffer, 0);
        }
    }
}


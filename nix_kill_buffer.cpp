
/////////////////////////////////////////////////////////////////////
// 01110000 01110011 01100100 01110010 01101110 01100100 01101101 //
///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// nick@psdrndm.com

// You need nix_get_buffers_opened_first.cpp in order to run this

// The only difference between this and the 4coder interactive_kill_buffer
// is that the current buffer is listed first, followed by buffers in other views

CUSTOM_UI_COMMAND_SIG(nix_interactive_kill_buffer)
CUSTOM_DOC("Interactively kill an open buffer.")
{
    Buffer_ID buffer = get_buffer_from_user_opened_in_views_first(app, "Kill:");
    if (buffer != 0){
        View_ID view = get_this_ctx_view(app, Access_Always);
        try_buffer_kill(app, buffer, view, 0);
    }
}

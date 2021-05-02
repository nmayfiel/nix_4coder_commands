
/////////////////////////////////////////////////////////////////////
// 01110000 01110011 01100100 01110010 01101110 01100100 01101101 //
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// nick@psdrndm.com

i64
nix_view_get_col_for_pos(Application_Links *app, View_ID view, i64 pos)
{
    Buffer_Cursor c = view_compute_cursor(app, view, seek_pos(pos));
    return (c.col);
}

i64
nix_view_get_line_for_pos(Application_Links *app, View_ID view, i64 pos)
{
    Buffer_Cursor c = view_compute_cursor(app, view, seek_pos(pos));
    return (c.line);
}


// @NOTE
// For kill, enter an empty string into query
// For replace, enter a string into query with cursor and mark in different columns
// Insert is a special case when cursor and mark are in the same column
// The right-most column is not included in the kill/replace
// Undo is broken, it goes one line at a time rather than all at once

// @TODO
// Handle the case where there is a line that doesn't extend fully into or past the
// specified rectangle
// Look for a better way to do this whole thing, it feels a bit hacky
// For insert and replace:
// Should whitespace be entered to make up for it? may be different answers for insert / replace

CUSTOM_UI_COMMAND_SIG(nix_rectangle_kill_insert_replace)
CUSTOM_DOC("Kill, replace or insert a rectangle bound by cursor and mark")
{
    Scratch_Block scratch(app);
    
    View_ID view = get_active_view(app, Access_ReadVisible);
    Buffer_ID buffer_id = view_get_buffer(app, view, Access_ReadVisible);
    if (!buffer_exists(app, buffer_id))
        return ;
    
    String_Const_u8 replacement_string;
    
    Query_Bar_Group group(app);
    Query_Bar replace = {};
    u8 replace_space[1024];
    replace.prompt = string_u8_litexpr("Insert/Replace: ");
    replace.string = SCu8(replace_space, (u64)0);
    replace.string_capacity = sizeof(replace_space);
    if (query_user_string(app, &replace)){
        if (replace.string.size > 0) {
            replacement_string = replace.string;
        }
        else
        {
            replacement_string = string_u8_empty;
        }
        
        ///////////////////////////////////////////////////////////
        // Do all the calculations for the rectangle
        ///////////////////////////////////////////////////////////
        
        i64 cpos = view_get_cursor_pos(app, view);
        i64 mpos = view_get_mark_pos(app, view);
        
        i64 ccol = nix_view_get_col_for_pos(app, view, cpos);
        i64 mcol = nix_view_get_col_for_pos(app, view, mpos);
        
        i64 cline = nix_view_get_line_for_pos(app, view, cpos);
        i64 mline = nix_view_get_line_for_pos(app, view, mpos);
        
        i64 top_left_pos = 0;
        i64 lines_to_kill = 0;
        i64 amount_to_kill = 0;
        i64 start_line     = 0;
        
        if (cpos > mpos){
            lines_to_kill = cline - mline + 1;
            start_line = mline;
            
            if (ccol > mcol){
                top_left_pos = mpos;
                amount_to_kill = ccol - mcol;
            } else {
                amount_to_kill = mcol - ccol;
                top_left_pos = mpos - amount_to_kill;
            }
        } else if (mpos > cpos){
            lines_to_kill = mline - cline + 1;
            start_line = cline;
            
            if (mcol > ccol){
                top_left_pos = cpos;
                amount_to_kill = mcol - ccol;
            } else {
                amount_to_kill = ccol - mcol;
                top_left_pos = cpos - amount_to_kill;
            }
        }
        
        ///////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////
        
        Rect_f32 r = view_padded_box_of_pos(app, view, start_line, top_left_pos);
        Vec2_f32 p = { r.x0, r.y1 };
        i64 this_pos = top_left_pos;
        
        for (int i = 0; i < lines_to_kill; ++i){
            Buffer_Cursor this_cursor = view_compute_cursor(app, view, seek_pos(this_pos));
            i64 end = this_pos + amount_to_kill;
            i64 buffer_size = buffer_get_size(app, buffer_id);
            if (0 < end && end <= buffer_size) {
                if (amount_to_kill > 0){
                    buffer_replace_range(app, buffer_id, Ii64(this_pos, end), replacement_string);
                } else {
                    Buffer_Insertion insert = begin_buffer_insertion_at(app, buffer_id, this_pos);
                    insertf(&insert, "%.*s", string_expand(replacement_string));
                    end_buffer_insertion(&insert);
                }
                
            }
            
            p.y += 1.0;
            this_pos = view_pos_at_relative_xy(app, view, this_cursor.line, p);
        }        
    }
}


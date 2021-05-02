
/////////////////////////////////////////////////////////////////////
// 01110000 01110011 01100100 01110010 01101110 01100100 01101101 //
///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// nick@psdrndm.com

function void
generate_all_buffers_list_opened_in_views_first(Application_Links *app, Lister *lister){
    lister_begin_new_item_set(app, lister);
    
    Buffer_ID viewed_buffers[16];
    i32 viewed_buffer_count = 0;
    
    // List the buffer in the current view
    {
        auto av = get_active_view(app, Access_Always);
        auto bid = view_get_buffer(app, av, Access_Always);
        if (!buffer_exists(app, bid))
            goto skipNix0;
        viewed_buffers[viewed_buffer_count++] = bid;
        skipNix0:;
    }
    
    // List currently viewed buffers
    for (View_ID view = get_view_next(app, 0, Access_Always);
         view != 0;
         view = get_view_next(app, view, Access_Always)){
        Buffer_ID new_buffer_id = view_get_buffer(app, view, Access_Always);
        for (i32 i = 0; i < viewed_buffer_count; i += 1){
            if (new_buffer_id == viewed_buffers[i]){
                goto skip0;
            }
        }
        viewed_buffers[viewed_buffer_count++] = new_buffer_id;
        skip0:;
    }
    
    // Buffers That Are Open in Views
    for (i32 i = 0; i < viewed_buffer_count; i += 1){
        generate_all_buffers_list__output_buffer(app, lister, viewed_buffers[i]);
    }
    
    // Regular Buffers
    for (Buffer_ID buffer = get_buffer_next(app, 0, Access_Always);
         buffer != 0;
         buffer = get_buffer_next(app, buffer, Access_Always)){
        for (i32 i = 0; i < viewed_buffer_count; i += 1){
            if (buffer == viewed_buffers[i]){
                goto skip1;
            }
        }
        if (!buffer_has_name_with_star(app, buffer)){
            generate_all_buffers_list__output_buffer(app, lister, buffer);
        }
        skip1:;
    }
    
    // Buffers Starting with *
    for (Buffer_ID buffer = get_buffer_next(app, 0, Access_Always);
         buffer != 0;
         buffer = get_buffer_next(app, buffer, Access_Always)){
        for (i32 i = 0; i < viewed_buffer_count; i += 1){
            if (buffer == viewed_buffers[i]){
                goto skip2;
            }
        }
        if (buffer_has_name_with_star(app, buffer)){
            generate_all_buffers_list__output_buffer(app, lister, buffer);
        }
        skip2:;
    }
    
    
}


function Buffer_ID
get_buffer_from_user_opened_in_views_first(Application_Links *app, String_Const_u8 query){
    Lister_Handlers handlers = lister_get_default_handlers();
    handlers.refresh = generate_all_buffers_list_opened_in_views_first;
    Lister_Result l_result = run_lister_with_refresh_handler(app, query, handlers);
    Buffer_ID result = 0;
    if (!l_result.canceled){
        result = (Buffer_ID)(PtrAsInt(l_result.user_data));
    }
    return(result);
}


function Buffer_ID
get_buffer_from_user_opened_in_views_first(Application_Links *app, char *query){
    return(get_buffer_from_user_opened_in_views_first(app, SCu8(query)));
}

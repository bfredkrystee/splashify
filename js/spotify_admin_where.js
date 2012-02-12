jQuery(document).ready(function(){
  if(jQuery("#edit-splashify-where-desktop-listpages").val() != '') {
    jQuery("#desktop_listpages").show();
  }

  jQuery("input[name='splashify_where_desktop_page']").change(function(){
    if(jQuery('#edit-splashify-where-desktop-page-list').is(':checked')){
      jQuery("#desktop_listpages").show();
    } else {
      jQuery("#desktop_listpages").hide();
      jQuery("#edit-splashify-where-desktop-listpages").val('');
    }
  });
});
$(document).ready(function($) {
  
    /* ------------------------- */
    /* Contact Form Interactions */
    /* ------------------------- */
    $('#contact').on('click', function(event) {
      event.preventDefault();

      $('.contact').addClass('is-visible');
  
      if ($('#song-box').val().length != 0) {
        $('.song-box').addClass('typing');
      }

    });
  
    //close popup when clicking x or off popup
    $('.addToList').on('click', function(event) {
      if ($(event.target).is('.cd-popup-close') || $(event.target).is('.addToList')) {
        event.preventDefault();
        $(this).removeClass('is-visible');
      }
      
    });
  
  });
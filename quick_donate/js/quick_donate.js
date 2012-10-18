/**
 * jQuery to support fundraiser behaviors.
 */
Drupal.behaviors.fundraiser = function(context) {

  // When other amount changes, make sure the amount radio is correctly set.
  $('#edit-other-amount').change(function() {
    if ($(this).val() != '') {
      $(':radio[value=other]').attr('checked', true);
    }
  });

  // On submission, deactivate the button and show processing message.
  $('.node .webform-client-form #edit-submit').click(function() {
    $(this).hide();
    $('.fundraiser_submit_message').hide();
    $(this).after('<div class="donation-processing-wrapper">'+
    '<p class="donation-thank-you">Thank you.</p><p class="donation-processing">Your donation is being processed.</p>'+
    '<div class="donation-processing-spinner"></div></div>');
  });

}

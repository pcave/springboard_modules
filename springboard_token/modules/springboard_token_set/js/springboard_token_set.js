(function ($) {
  Drupal.behaviors.SpringboardTokenSet = {
    attach: function (context, settings) {
      // Add token click handler to form elements with token set IDs.
      $(".form-wrapper, #webform-component-edit-form #edit-value, #webform-component-edit-form #edit-extra-description").each(function() {
        var token_set_id = $(this).data("token-set-id");
        if (typeof token_set_id !== typeof undefined) {
          $(this).addClass("has-token-data");
          var targetElement = $(this).find("textarea");
          if (targetElement.length == 0) {
            targetElement = $(this);
          }
          targetElement.filter("textarea").click(function() {
            Drupal.settings.token_set_last_selected_field = this;
            showTokens($(this));
            $('.first-token-set').click();
            $('.first-token-set').removeClass('first-token-set');
          });
        }
      });

      var showTokens = function (element) {
        // Remove existing tokens div.
        $("#token-set-tokens").remove();

        var token_set_id = element.closest(".has-token-data").data("token-set-id");
        if (Drupal.settings["token_sets_" + token_set_id] == undefined) {
          var tokens = Drupal.settings.token_sets[token_set_id];
        }
        else {
          var tokens = Drupal.settings["token_sets_" + token_set_id];
        }

        // Generate tokens markup.
        var last_token_type = '';
        var headerHTML = '<div class="token-ui-header">';
        var html = '<fieldset id="token-set-tokens" class="form-wrapper">';
        html += '<div class="token-set-tokens-spike">&nbsp;</div>';
        html += '<div class="token-set-tokens-inner">';
        html += '<legend><span class="fieldset-legend">Tokens</span></legend>';
        html += '<div class="fieldset-wrapper token-set-wrapper">';
        html += '<div class="token-list">';
        var firstTokenSet = ' first-token-set';
        for (var i = 0; i < tokens.length; i++) {
          if (tokens[i].token_type !== last_token_type) {
            headerHTML += '<a class="token-set-expand' + firstTokenSet + '" data-type="' + tokens[i].token_type.replace(' ', '-') + '" data-expanded="0" href="#">' + tokens[i].token_type + '</a>';
            last_token_type = tokens[i].token_type;
          }
          firstTokenSet = '';
          html += renderToken(tokens[i]);
        }
        headerHTML += '</div>';
        html += '</div></div></div>';
        html += '</fieldset>';
        element.parent().append(html);
        $('#token-set-tokens legend').after(headerHTML);
        $(".token-set-expand").each(function() {
          $(this).click(function(event) {
            event.preventDefault();
            var token_type = $(this).data("type");
            $(".token-set-token-row").hide();
            $(".token-set-token-type-" + token_type.replace(' ', '-')).show();
            $(".token-set-expand").removeClass("token-set-expanded");
            $(this).addClass("token-set-expanded");
          });
        });

        $('.token-set-token-row').each(function() {
          $(this).click(function(event) {
            event.preventDefault();
            var token = $(this).data("token");
            insertToken(Drupal.settings.token_set_last_selected_field, token);
          });
        });
      };

      $('.token-set-token-row').click(function () {
        event.preventDefault();
        $(this).children('a.token-set-token-link').click();
      });


      var renderToken = function (token) {
        var html = '<div class="token-set-token-row token-set-token-type-' + token.token_type.replace(' ', '-') + '" data-token="' + token.token + '">';
        html += '<div class="token-col token-machine">' + token.token + '</div>';
        html += '<div class="token-col token-descr">' + token.token_description + '</div>';
        html += '<div class="token-col token-use">use token</div>';
        html += '</div>';
        return html;
      };

      var insertToken = function (element, token) {
        // IE support.
        if (document.selection) {
          element.focus();
          sel = document.selection.createRange();
          sel.text = token;
        }
        // Mozilla support.
        else if (element.selectionStart || (element.selectionStart === '0')) {
          element.value = element.value.substring(0, element.selectionStart)
            + token
            + element.value.substring(element.selectionEnd, element.value.length);
        } else {
          element.value += token;
        }
      };
    }
  };
})(jQuery);

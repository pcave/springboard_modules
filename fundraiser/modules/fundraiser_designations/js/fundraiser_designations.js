(function ($) {

  Drupal.behaviors.fundraiserDesignations = {
    attach: function(context, settings) {
        new Drupal.fundraiserDesignations();
    }
  };

  /**
   * Set the amounts on page load
   */
  Drupal.fundraiserDesignations = function() {
    this.fundGroups = $('.designation-group-wrapper');
    this.cart = $('.fundraiser-designation-cart-wrapper');
    this.cartTemplate = '<tr class="cart-fund-row"><td class="fund-cancel">x</td><td class="fund-name"></td><td class="fund-amount"></td> </tr>';
    this.errorTemplate = '<div class="error-message"></div>';
    this.addListeners();
    this.cancelButton();
    var ct = $('#cart_total');
    var cart_total = ct.val();
    var val = cart_total > 0 ? cart_total : 0;
    ct.val(Drupal.settings.fundraiser.currency.symbol + (val).formatMoney(2, '.', ','));

    var selectContain = $('.designation-group-funds-table div.form-type-select');
    var selectContainWidth = 0;
    selectContain.each(function() {
     if ($(this).width() > selectContainWidth) {
       selectContainWidth = $(this).width();
     }
   });

    var fundContain = $('.designation-group-funds-table div[id*="funds-placeholder"]');
    var fundContainWidth = 0;
    fundContain.each(function() {
      if ($(this).width() > fundContainWidth) {
        fundContainWidth = $(this).width();
      }
    });

    var wide = fundContainWidth > selectContainWidth ? fundContainWidth : selectContainWidth;
    selectContain.css('width', wide);
    fundContain.css('width', wide);
  };

  /**
   * Set the amounts on select
   */
  // Iterate over each field in the settings and add listener
  Drupal.fundraiserDesignations.prototype.addListeners = function() {

    var self = this;
    $.each(self.fundGroups, function(key, item) {

      var fundGroup = $(item);
      var fundGroupId = item['id'].replace('designation-group-', '')
      var selector = fundGroup.find('select[name*="funds_select"]');
      var defaultAmts = $('div[id*="default-amounts-"]', fundGroup);
      var recurAmts = $('div[id*="recurring-amounts-"]', fundGroup);
      var otherAmt = $('input[type="text"]', fundGroup);
      var quantity = fundGroup.find('select[name*="funds_quant"]');


      $(window).ready(function () {
        self.validateOtherAmt(fundGroupId);
      });

      otherAmt.keyup(function() {
        $('input[type="radio"]', fundGroup).each(function(){
          $(this).prop('checked', false);
        })
      });

      otherAmt.blur(function() {
        var message = $('label.error').text();
        $('label.error').remove();
      });

      $.each($('input[type="radio"]', fundGroup), function(){
        $(this).on('click', function(){
          otherAmt.val('');
        });
      });

      $('input[type="submit"]', fundGroup).click(function() {
        button = $(this);
        self.addFund(button, fundGroupId, selector, defaultAmts, recurAmts, otherAmt, quantity);
        return false;
      });
    });
  };

  Drupal.fundraiserDesignations.prototype.addFund = function(button, fundGroupId, selector, defaultAmts, recurAmts, otherAmt, quantity) {
    var self = this;

    var go = self.validateFund(fundGroupId, selector, defaultAmts, recurAmts, otherAmt);

    if (go != 'ok') {
      return;
    }

    var amt = 0;
    if (defaultAmts.length > 0 && defaultAmts.is(':visible')) {
      amt = $(defaultAmts).find('input:checked').val();
    }
    if (recurAmts.length > 0 && recurAmts.is(':visible')) {
      amt = $(recurAmts).find('input:checked').val();
    }
    if (otherAmt.val()) {
      amt = otherAmt.val();
    }

    var quant = 1;
    var displayAmt = Drupal.settings.fundraiser.currency.symbol + (parseInt(amt)).formatMoney(2, '.', ',');

    var displayQuant = '';
    if (quantity.length > 0) {
      quant = quantity.val();
      displayAmt = amt * quant;
      displayAmt = Drupal.settings.fundraiser.currency.symbol + (parseInt(displayAmt)).formatMoney(2, '.', ',');
      if (quant > 1) {
        displayQuant = ' (' + quant + ' x ' + Drupal.settings.fundraiser.currency.symbol + amt + ')';
      }
    }

    if (selector.length > 0) {
      var fundId = selector.val()
      var fundName = $('option:selected', selector).text();
    }
    else {
       fundId = $('tr.group-row-' + fundGroupId).attr('data-placeholder-fund-id')
       fundName = $('#funds-placeholder-' + fundGroupId).find('label').text();
    }

    var newRow = $(self.cartTemplate);
    $('.fund-amount', newRow).text(displayAmt)
    $('.fund-name', newRow).text(fundName + displayQuant);
    newRow.attr('data-fund-id', fundId);
    newRow.attr('data-fund-amount', amt);
    newRow.attr('data-fund-quantity', quant);

    var exists = false;
    $('tr', self.cart).each(function(){
      if($(this).attr('data-fund-id') == fundId && $(this).attr('data-fund-amount') == amt) {
        var oldAmt = parseInt($('.fund-amount', $(this)).text().replace('$', ''));
        var oldQuant = $(this).attr('data-fund-quantity');
        var newQuant = parseInt(oldQuant) + parseInt(quant);
        var newAmt = parseInt($('.fund-amount', newRow).text().replace('$', '')) + oldAmt;
        newRow.attr('data-fund-quantity', newQuant);

        if (newQuant > 1) {
          displayQuant = ' (' +  newQuant + ' x ' + Drupal.settings.fundraiser.currency.symbol + amt + ')';
        }

        $('.fund-name', newRow).text(fundName + displayQuant);

        $('.fund-amount', newRow).text(Drupal.settings.fundraiser.currency.symbol + (parseInt(newAmt)).formatMoney(2, '.', ','))

        $(this).replaceWith(newRow);
        exists = true;
      }
    });

    if (!exists) {
      newRow.insertBefore('.cart-total-row').hide().show(300);
    }

    self.setFormValue();

    if ($(".cart-fund-empty").is(':visible')) {
      $(".cart-fund-empty").hide();
    }
    self.cancelButton();
    self.setAmounts();

  }

  // attaches JSONified data attributes of the recipients list to a hidden form field
  Drupal.fundraiserDesignations.prototype.setFormValue = function () {
    var obj = {};
    $('.cart-fund-row').each(function(i) {
      obj[i] = $(this).data();
    });
    var lineItems = JSON.stringify(obj).replace(/"/g, '&quot;');
    console.log(lineItems);

    $('input[name$="[fund_catcher]"]').val(lineItems);

  };

  Drupal.fundraiserDesignations.prototype.validateOtherAmt = function(groupId) {
      if ($('#fd-other-' + groupId)[0]) {
        $('#fd-other-' + groupId).rules("add", {
          amount: true,
          min: parseFloat(Drupal.settings.fundraiserWebform.minimum_donation_amount),
          messages: {
            required: "This field is required",
            amount: "Enter a valid amount",
            min: "The amount entered is less than the minimum donation amount."
          }
        });
      }
  };

  Drupal.fundraiserDesignations.prototype.validateFund = function(fundGroupId, selector, defaultAmts, recurAmts, otherAmt) {
    var self = this;
    var fundSel = selector.val() != 0 ? true : false;
    if (defaultAmts.length > 0 && defaultAmts.is(':visible')) {
      var amt = $(defaultAmts).find('input:checked').val();
    }
    if (recurAmts.length > 0 && recurAmts.is(':visible')) {
      amt = $(recurAmts).find('input:checked').val();
    }
    if (otherAmt.val()) {
      amt = otherAmt.val();
    }
    if (typeof(amt) !== 'undefined') {
      var amtSel = true;
    }
    else {
      amtSel = false;
    }

    var message = 'ok';
    if (!fundSel &&!amtSel) {
      message = 'Please choose a fund and select an amount.';
    }
    else if(!fundSel) {
      message = 'Please choose a fund.';
    }
    else if(!amtSel) {
      message = 'Please select an amount'
    }

    if (amtSel) {
      var amtFloat = parseFloat(amt);
      if (Number.isNaN(amtFloat) || !$.isNumeric(amt)) {
        message += ' Not a valid amount.';
      }
      else {
        var MinAmt = parseFloat(Drupal.settings.fundraiserWebform.minimum_donation_amount);
        if (amtFloat < MinAmt) {
          message += ' Below the minumum amount.'
        }
      }
    }

    var errorContain = $("#designation-group-" + fundGroupId + " .designation-group-title");
    if (message != 'ok') {
      var error = $(self.errorTemplate).text(message);
        errorContain.find('.error-message').remove();
        errorContain.append(error)
    }
    else {
      errorContain.find('.error-message').remove();
    }

    return message;
  }

  /**
   * Set the amounts on select
   */
    // Iterate over each field in the settings and add listener
  Drupal.fundraiserDesignations.prototype.cancelButton = function() {
    var self = this;
    $.each($('tr', self.cart), function(key, row) {
      $('.fund-cancel', this).click(function() {
         $(row).hide(300, function(){
           $(row).remove();
           self.setAmounts();
           if ($('tr.cart-fund-row', self.cart).length == 1) {
             $(".cart-fund-empty").show(100);
           }
         });
      });
    });
  };

  /**
   * Update the various HTML elements with our amounts
   */
  Drupal.fundraiserDesignations.prototype.setAmounts = function() {
    var self = this,
    total = self.calcTotal();
    $('#cart_total').val(Drupal.settings.fundraiser.currency.symbol + (total).formatMoney(2, '.', ','));
    $("input[name='submitted[amount]']").val((total).formatMoney(2, '.', ''));
  }

  /**
   * Calculate the total amount
   */
  Drupal.fundraiserDesignations.prototype.calcTotal = function() {
    var self = this;
    var total = 0;
    $.each($('td.fund-amount', self.cart), function(i, price) {
      total = total + parseInt($(price).text().replace('$', ''));
    });
    return total;
  }

  /**
   * Format values as money
   *
   * http://stackoverflow.com/a/149099/1060438
   */
  Number.prototype.formatMoney = function(c, d, t) {
    var n = this, c = isNaN(c = Math.abs(c)) ? 2 : c, d = d == undefined ? "," : d, t = t == undefined ? "." : t, s = n < 0 ? "-" : "", i = parseInt(n = Math.abs(+n || 0).toFixed(c)) + "", j = (j = i.length) > 3 ? j % 3 : 0;
    return s + (j ? i.substr(0, j) + t : "") + i.substr(j).replace(/(\d{3})(?=\d)/g, "$1" + t) + (c ? d + Math.abs(n - i).toFixed(c).slice(2) : "");
  };

})(jQuery);
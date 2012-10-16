<div class="quick-donate-payment-method">
<?php
if ($payment_method):
?>

<p class="card-details">
<?php print "$cc_type - $cc_number"; ?>
</p>
<p class="expiration">
<?php print "Expires $cc_exp_month/$cc_exp_year" ?>
</p>

<p class="name">
<?php print "$billing_first_name $billing_last_name"; ?>
</p>
<p class="address-1">
<?php print $billing_street1; ?>
</p>
<?php
if (isset($billing_street2)):
?>
<p class="address-2">
<?php print $billing_street2; ?>
</p>
<?php
endif;
?>
<p class="city-state-zip">
<?php print "$billing_city $billing_state, $billing_postal_code"; ?>
<p>
<?php
if (isset($payment_connections)):
  print $payment_connections;
endif;
?>
<?php
endif;
?>
<div class="payment-method-links">
<?php print $payment_method_links; ?>
</div>
</div>

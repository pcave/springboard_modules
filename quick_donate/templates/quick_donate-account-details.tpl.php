<div class="quick-donate-account-details">

<?php if (isset($billing_first_name) || isset($billing_last_name)) : ?>  
  <h3 class="qd-acct-lede">Welcome back <?php print $billing_first_name?>. Choose your amount and click on the submit button to process your donation.</h3>
  <p class="qd-name"><?php print "$billing_first_name $billing_last_name"; ?></p>
<?php endif; ?>
<?php if (isset($billing_street1)) : ?>
  <p class="qd-addr1"><?php print $billing_street1; ?></p>
<?php endif ?>
<?php if (isset($billing_street2)) : ?>
  <p class="qd-addr2"><?php print $billing_street2; ?></p>
<?php endif; ?>
<?php if (isset($billing_city) || isset($billing_zone) || isset($billing_postal_code)) : ?>
  <p class="qd-location"><?php print "$billing_city, $billing_zone $billing_postal_code"; ?></p>
<?php endif; ?>
<?php
if (isset($links)) {
  foreach ($links as $link) {
?>

<p class="qd-acct-links"><?php print $link; ?></p>

<?php
  }
}
 ?>

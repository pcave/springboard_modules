<?php

/**
 * @file
 * Used to display a welcome message when a webform is accessed using an encrypted query string.
 *
 * This file may be renamed "secure_prepopulate_welcome_[nid].tpl.php" to target a
 * specific webform on your site. Or you can leave it
 * "secure_prepopulate_welcome.tpl.php" to affect all webforms on your site.
 *
 * Available variables:
 * - $values: An array of key/value pairs that were passed in via query string.
 * - $nid: The nid of the node for which the values apply.
 */
?>
<?php if (isset($welcome_prefix)) : ?>
<div id="welcome-prefix">
  <?php print $welcome_prefix; ?>
</div>
<?php endif; ?>
<div id="welcome-back">
  <div class="salutation">
    <?php print $welcome_message; ?>
  </div>
  <div class="">
    <?php print $not_me; ?>
  </div>
<?php if (isset($welcome_suffix)) : ?>
<div id="welcome-suffix">
  <?php print $welcome_suffix; ?>
</div>
<?php endif; ?>
</div>

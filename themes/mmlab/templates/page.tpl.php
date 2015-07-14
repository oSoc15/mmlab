<?php
/**
 * @file
 * Returns the HTML for a single Drupal page.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728148
 */
?>

<div id="page">

  <header class="header" id="header" role="banner">

    <?php if ($logo): ?>
      <a href="http://www.ugent.be" title="<?php print t('Home'); ?>" rel="home" class="header__logo" id="logo"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" class="header__logo-image" /></a>
    <?php endif; ?>

    <?php if ($site_name || $site_slogan): ?>
      <div class="header__name-and-slogan" id="name-and-slogan">
        <?php if ($site_name): ?>
          <h1 class="header__site-name" id="site-name">
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" class="header__site-link no-deco" rel="home"><span class="white-text" id="header-name"><?php print $site_name; ?></span></a>
          </h1>
        <?php endif; ?>

        <?php if ($site_slogan): ?>
          <div class="header__site-slogan" id="site-slogan"><?php print $site_slogan; ?></div>
        <?php endif; ?>
      </div>
    <?php endif; ?>

      <div id="navigation">

          <?php if ($main_menu): ?>
              <nav id="main-menu" role="navigation" tabindex="-1">
                  <?php
                  // This code snippet is hard to modify. We recommend turning off the
                  // "Main menu" on your sub-theme's settings form, deleting this PHP
                  // code block, and, instead, using the "Menu block" module.
                  // @see https://drupal.org/project/menu_block
                  print theme('links__system_main_menu', array(
                      'links' => $main_menu,
                      'attributes' => array(
                          'class' => array('links', 'inline', 'clearfix'),
                      ),
                      'heading' => array(
                          'text' => t('Main menu'),
                          'level' => 'h2',
                          'class' => array('element-invisible'),
                      ),
                  )); ?>
              </nav>
          <?php endif; ?>

          <?php print render($page['navigation']); ?>

      </div>

    <?php if (/*$secondary_menu*/1==0): ?>
      <nav class="header__secondary-menu" id="secondary-menu" role="navigation">
        <?php print theme('links__system_secondary_menu', array(
          'links' => $secondary_menu,
          'attributes' => array(
            'class' => array('links', 'inline', 'clearfix'),
          ),
          'heading' => array(
            'text' => $secondary_menu_heading,
            'level' => 'h2',
            'class' => array('element-invisible'),
          ),
        )); ?>
      </nav>
    <?php endif; ?>

    <?php print render($page['header']); ?>

      <div id="block-search-form" class="block block-search contextual-links-region first last odd" role="search">

          <form action="/mmlab/mmlab/?q=node" method="post" id="search-block-form" accept-charset="UTF-8"><div><div class="container-inline">
                      <h2 class="element-invisible">Search form</h2>
                      <div class="form-item form-type-textfield form-item-search-block-form">
                          <label class="element-invisible" for="edit-search-block-form--2">Search </label>
                          <input title="Enter the terms you wish to search for." type="text" id="edit-search-block-form--2" name="search_block_form" value="" size="15" maxlength="128" class="form-text header-searchfield">
                      </div>
                      <div class="form-actions form-wrapper" id="edit-actions"><input type="submit" id="edit-submit" name="op" value="&#x1f50d;" class="form-submit header-searchbutton"></div><input type="hidden" name="form_build_id" value="form-GqbOrXE49kcVbiXG-GqwsHv8YqO5Bol9E4Yx10PZDew">
                      <input type="hidden" name="form_token" value="5GAf_Y7CyIQ4p17v82eJnGZqV847nvnfjJcKDRwsVc4">
                      <input type="hidden" name="form_id" value="search_block_form">
                  </div>
              </div></form>
      </div>

  </header>

  <div id="main">

    <div id="content" class="column" role="main">
      <?php print render($page['highlighted']); ?>
      <?php print $breadcrumb; ?>
      <a id="main-content"></a>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 class="page__title title" id="page-title"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php print $messages; ?>
      <?php print render($tabs); ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>
      <?php print render($page['content']); ?>
      <?php print $feed_icons; ?>
    </div>



    <?php
      // Render the sidebars to see if there's anything in them.
      $sidebar_first  = render($page['sidebar_first']);
      $sidebar_second = render($page['sidebar_second']);
    ?>

    <?php if ($sidebar_first || $sidebar_second): ?>
      <aside class="sidebars">
        <?php print $sidebar_first; ?>
        <?php print $sidebar_second; ?>
      </aside>
    <?php endif; ?>

  </div>

  <?php print render($page['footer']); ?>

</div>

<?php print render($page['bottom']); ?>

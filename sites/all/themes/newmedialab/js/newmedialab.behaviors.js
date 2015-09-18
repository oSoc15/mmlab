(function ($) {

  /**
   * The recommended way for producing HTML markup through JavaScript is to write
   * theming functions. These are similiar to the theming functions that you might
   * know from 'phptemplate' (the default PHP templating engine used by most
   * Drupal themes including Omega). JavaScript theme functions accept arguments
   * and can be overriden by sub-themes.
   *
   * In most cases, there is no good reason to NOT wrap your markup producing
   * JavaScript in a theme function.
   */
  Drupal.theme.prototype.newMediaLabExampleButton = function (path, title) {
    // Create an anchor element with jQuery.
    return $('<a href="' + path + '" title="' + title + '">' + title + '</a>');
  };

  /**
   * Behaviors are Drupal's way of applying JavaScript to a page. In short, the
   * advantage of Behaviors over a simple 'document.ready()' lies in how it
   * interacts with content loaded through Ajax. Opposed to the
   * 'document.ready()' event which is only fired once when the page is
   * initially loaded, behaviors get re-executed whenever something is added to
   * the page through Ajax.
   *
   * You can attach as many behaviors as you wish. In fact, instead of overloading
   * a single behavior with multiple, completely unrelated tasks you should create
   * a separate behavior for every separate task.
   *
   * In most cases, there is no good reason to NOT wrap your JavaScript code in a
   * behavior.
   *
   * @param context
   *   The context for which the behavior is being executed. This is either the
   *   full page or a piece of HTML that was just added through Ajax.
   * @param settings
   *   An array of settings (added through drupal_add_js()). Instead of accessing
   *   Drupal.settings directly you should use this because of potential
   *   modifications made by the Ajax callback that also produced 'context'.
   */
  Drupal.behaviors.newMediaLabExampleBehavior = {
    attach: function (context, settings) {
//<<<<<<< HEAD

/*
	$('.group_event_pub').accordion({collapsible:true});
	$('.group_event_pub .field-group-format-title').text('Publications');
	console.log($('.group_event_pub .view-content div').lengt);
	if($('.group_event_pub .view-content div').length === 0){
		$('.group_event_pub').hide();
	}
//<<<<<<< HEAD


	$('.group-topic-proj').accordion({collapsible: true, active: false});
        $('.group-topic-proj .field-group-format-title').text('Projects');
        console.log($('.group-topic-proj .view-content div').lengt);
        if($('.group-topic-proj .view-content div').length === 0){
                $('.group-topic-proj').hide();
        }
*/	
	/*
	* Publication view 
	*
	*/	
	var pubs = $('.biblio-title');
	// Replace on view page change
	jQuery(document).ajaxComplete(function(){        
        var pubs = $('.biblio-title');
		var str = "";
		// Hide the braces on display
		for (i = 0; i < pubs.length; i++) { 
			str = pubs[i].getElementsByTagName('a')[0].innerHTML;
			str = str.replace("{", "");
			str = str.replace("}", "");
			pubs[i].getElementsByTagName('a')[0].innerHTML = str;
		}                                   
    });
	// Hide filters when there is no publication.
	if(pubs.length < 2) {
		 $('.view-filters').hide();
		 // If there is only one Publication change title from Publications to Publication.
		 if(pubs.length == 1) {
			$('.view-header h3').text("Publication");
		 }
	}
	
	
	/*
	* Twitter feeds 
	*
	*/
	var twitter = document.getElementsByClassName('field-name-field-twitter').length;
	// hide feeds block if no twitter account.
	if (twitter < 1) {
		$('#block-widgets-s-twitter-user-timeline-widget').hide();
	}
	
	/*
	* About us: description link
	*
	*/
	
	var aboutpage = $('#node-82');
	var routefield = $( ".field-group-format-title");
	if (aboutpage.length >= 1) {
		routefield.click(function() {
			if (routefield[0].innerHTML == "Show route description") {
				routefield.text("Hide route description");
			} else {
				routefield.text("Show route description");
			}
		});
	}

//=======
//>>>>>>> parent of d8ad178... #98 events publications are now collapsible trough jquery ui accordion
//=======
//>>>>>>> parent of 7406cfa... #98 topics publications and topics are now collapsible
      // By using the 'context' variable we make sure that our code only runs on
      // the relevant HTML. Furthermore, by using jQuery.once() we make sure that
      // we don't run the same piece of code for an HTML snippet that we already
      // processed previously. By using .once('foo') all processed elements will
      // get tagged with a 'foo-processed' class, causing all future invocations
      // of this behavior to ignore them.
      $('.some-selector', context).once('foo', function () {
        // Now, we are invoking the previously declared theme function using two
        // settings as arguments.
        var $anchor = Drupal.theme('newMediaLabExampleButton', settings.myExampleLinkPath, settings.myExampleLinkTitle);

        // The anchor is then appended to the current element.
        $anchor.appendTo(this);
      });
    }
  };

})(jQuery);

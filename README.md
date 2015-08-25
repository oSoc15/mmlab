# mmlab
New website for MMLab.be. Drupal-based with RDF extensions and possibilities to export it in a linked data format (turtle, rdf+xml, jsonld).

How to install it locally to develop with us?
Git clone this repo, switch to origin/dev branch.
Put the latest sql dump from the sqldumps directory in your database.

Run the Drupal setup (you probably need to create a settings.php and a "files" directory).
Change the permissions on settings.php and files.

Then generate the composer.json file with the composer manager module.
Run composer install

and you're good to go!!

If you want to add changes just send us a pull request...

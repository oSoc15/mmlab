<?php
/**
 * @file
 * mmlab_content_types.features.inc
 */

/**
 * Implements hook_node_info().
 */
function mmlab_content_types_node_info() {
  $items = array(
    'event' => array(
      'name' => t('Event'),
      'base' => 'node_content',
      'description' => t('An event organised by MMLab'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'partner' => array(
      'name' => t('Partner'),
      'base' => 'node_content',
      'description' => t('Content type for adding a partner'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'person' => array(
      'name' => t('Person'),
      'base' => 'node_content',
      'description' => t('A person working at MMLab'),
      'has_title' => '1',
      'title_label' => t('Full name'),
      'help' => '',
    ),
    'project' => array(
      'name' => t('Project'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'publications' => array(
      'name' => t('Publication'),
      'base' => 'node_content',
      'description' => t('Content type for publications'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'research_topic' => array(
      'name' => t('Research topic'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Topic Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

/**
 * Implements hook_rdf_default_mappings().
 */
function mmlab_content_types_rdf_default_mappings() {
  $schemaorg = array();

  // Exported RDF mapping: person
  $schemaorg['node']['person'] = array(
    'rdftype' => array(
      0 => 'schema:Person',
      1 => 'sioc:Item',
      2 => 'foaf:Document',
      3 => 'foaf:Person',
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
      'type' => 'property',
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'schema:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'field_affiliation' => array(
      'predicates' => array(
        0 => 'schema:affiliation',
      ),
    ),
    'field_job_title' => array(
      'predicates' => array(
        0 => 'schema:jobTitle',
      ),
      'type' => 'property',
      'datatype' => 'rdfs:Literal',
    ),
    'url' => array(
      'predicates' => array(
        0 => 'schema:url',
      ),
      'type' => 'rel',
    ),
    'field_person' => array(
      'predicates' => array(
        0 => 'schema:Person',
      ),
    ),
    'field_name' => array(
      'predicates' => array(
        0 => 'foaf:familyName',
      ),
      'type' => 'property',
      'datatype' => 'xsd:string',
    ),
    'field_email' => array(
      'predicates' => array(
        0 => 'schema:email',
        1 => 'foaf:email',
      ),
      'type' => 'property',
      'datatype' => 'xsd:string',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'schema:name',
      ),
      'type' => 'property',
    ),
    'field_first_name' => array(
      'type' => 'property',
      'datatype' => 'rdfs:Literal',
      'predicates' => array(
        0 => 'schema:givenName',
        1 => 'foaf:firstName',
      ),
    ),
    'field_last_name' => array(
      'type' => 'property',
      'datatype' => 'rdfs:Literal',
      'predicates' => array(
        0 => 'schema:familyName',
        1 => 'foaf:lastName',
      ),
    ),
    'field_picture' => array(
      'type' => 'rel',
      'datatype' => 'img',
      'predicates' => array(
        0 => 'schema:image',
        1 => 'foaf:Image',
      ),
    ),
    'field_phone' => array(
      'type' => 'property',
      'datatype' => 'rdfs:Literal',
      'predicates' => array(
        0 => 'schema:telephone',
        1 => 'foaf:phone',
      ),
    ),
    'field_facebook' => array(
      'type' => 'property',
      'predicates' => array(
        0 => 'foaf:homepage',
      ),
    ),
    'field_twitter' => array(
      'type' => 'property',
      'datatype' => 'rdfs:Literal',
      'predicates' => array(
        0 => 'foaf:homepage',
      ),
    ),
    'field_linkedin' => array(
      'type' => 'property',
      'datatype' => 'rdfs:Literal',
      'predicates' => array(
        0 => 'schema:homepage',
        1 => 'foaf:homepage',
      ),
    ),
    'field_about' => array(
      'type' => 'property',
      'datatype' => 'rdfs:Literal',
      'predicates' => array(
        0 => 'schema:about',
        1 => 'foaf:about',
      ),
    ),
  );

  // Exported RDF mapping: project
  $schemaorg['node']['project'] = array(
    'rdftype' => array(
      0 => 'schema:Project',
      1 => 'sioc:Item',
      2 => 'foaf:Document',
      3 => 'foaf:Project',
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'schema:description',
        1 => 'content:encoded',
        2 => 'foaf:description',
      ),
      'type' => 'property',
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'schema:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'schema:name',
      ),
      'type' => 'property',
    ),
    'url' => array(
      'predicates' => array(
        0 => 'schema:url',
      ),
      'type' => 'rel',
    ),
    'field_logo' => array(
      'type' => 'rel',
      'predicates' => array(
        0 => 'schema:image',
        1 => 'foaf:image',
      ),
    ),
    'field_project_website' => array(
      'type' => 'property',
      'datatype' => 'rdfs:Literal',
      'predicates' => array(
        0 => 'schema:homepage',
        1 => 'foaf:homepage',
      ),
    ),
    'field_demo' => array(
      'type' => 'property',
      'datatype' => 'rdfs:Literal',
      'predicates' => array(
        0 => 'schema:homepage',
        1 => 'foaf:homepage',
      ),
    ),
    'field_people_involved' => array(
      'type' => 'property',
      'predicates' => array(
        0 => 'schema:Person',
        1 => 'foaf:Person',
      ),
    ),
    'field_partners_involved' => array(
      'predicates' => array(),
    ),
    'field_partners' => array(
      'type' => 'property',
      'predicates' => array(
        0 => 'schema:partner',
        1 => 'foaf:partner',
      ),
    ),
  );

  // Exported RDF mapping: research_topic
  $schemaorg['node']['research_topic'] = array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document',
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded',
      ),
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator',
      ),
      'type' => 'rel',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'schema:name',
      ),
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies',
      ),
      'datatype' => 'xsd:integer',
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date',
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601',
    ),
    'title' => array(
      'predicates' => array(
        0 => 'schema:name',
      ),
    ),
    'field_short_description' => array(
      'type' => 'property',
      'datatype' => 'rdfs:Literal',
      'predicates' => array(
        0 => 'schema:description',
        1 => 'foaf:description',
      ),
    ),
    'url' => array(
      'predicates' => array(
        0 => 'schema:url',
      ),
      'type' => 'rel',
    ),
    'field_people_involved_topic' => array(
      'type' => 'property',
      'predicates' => array(
        0 => 'schema:person',
        1 => 'foaf:person',
      ),
    ),
    'field_relevant_publications' => array(
      'type' => 'property',
      'predicates' => array(
        0 => 'schema:publications',
        1 => 'foaf:publications',
      ),
    ),
  );

  return $schemaorg;
}

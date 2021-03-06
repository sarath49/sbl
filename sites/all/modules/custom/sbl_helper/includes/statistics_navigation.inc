<?php

/**
 * Functions for the statistics navigation block
 */
 
 function check_current_league_of_player() {
 $player=arg(1);
 $season=arg(2);
 
$result = db_select('field_data_field_sp_verein', 'f')
    ->fields('f')
    ->condition('entity_id', $player,'=')
    ->execute()
    ->fetchAll();
	
	for($i=0;$i<count($result);$i++) {
	$values[$i]=$result[$i]->field_sp_verein_value;
}


$result = db_select('field_data_field_saison', 'f')
    ->fields('f')
    ->condition('field_saison_tid', $season,'=')
	->condition('entity_id', $values,'IN')
    ->execute()
    ->fetchAll();

	$league_field_id=$result[0]->entity_id;


$result = db_select('field_data_field_pl_league', 'f')
    ->fields('f')
    ->condition('entity_id', $league_field_id,'=')
    ->execute()
    ->fetchAll();

$league_id=$result[0]->field_pl_league_tid;

return $league_id;
}

 
 function check_current_league_of_club() {
 $club=arg(1);
 $season=arg(2);
 
$result = db_select('field_data_field_ligenzuordnung', 'f')
    ->fields('f')
    ->condition('entity_id', $club,'=')
    ->execute()
    ->fetchAll();
	
	for($i=0;$i<count($result);$i++) {
	$values[$i]=$result[$i]->field_ligenzuordnung_value;
}

$result = db_select('field_data_field_liga_saison', 'f')
    ->fields('f')
    ->condition('field_liga_saison_tid', $season,'=')
	->condition('entity_id', $values,'IN')
    ->execute()
    ->fetchAll();

	$league_field_id=$result[0]->entity_id;
	
$result = db_select('field_data_field_liga_verein', 'f')
    ->fields('f')
    ->condition('entity_id', $league_field_id,'=')
    ->execute()
    ->fetchAll();

$league_id=$result[0]->field_liga_verein_tid;

return $league_id;
}
 
 function statistics_check_type_of_page() {
	 

  // Find out on which page of our statistics area we are.
  // There are some panels pages
  $page_type = '';
  if (panels_get_current_page_display()) {
    $page_type = 'panel_';
    switch(arg(0)) {
      case 'verein':
        $page_type .= 'verein';
        break;
      case 'spieler':
        $page_type .= 'spieler';
        break;
      case 'ergebnisse':
        $page_type .= 'ergebnisse';
        break;
	 case 'tabelle':
        $page_type .= 'tabelle';
        break;
	case 'tabelle-liga-2':
        $page_type .= 'tabelle';
        break;
    }
  }
  // and some views
  $views_page = views_get_page_view(); // Does this need a check in order not to throw an error?
  if (is_object($views_page)) {
    $page_type = 'view_' . $views_page->name;
  }
  // return as a variable where we are.

  return $page_type;
}

function statistics_create_navigation() {
  // Create a navigation block to get to the statistics area we want

  // Set default values for season and matchday
  $season = get_current_season();
  $matchday = get_current_matchday();
  //Get the season from the url arguments
  switch(statistics_check_type_of_page()) {
    case ('panel_verein'):
	$season=arg(2);
	$league=check_current_league_of_club();
	break;
    case ('panel_spieler'):
      $season = arg(2);
	  $league = check_current_league_of_player();
      break;
    case 'panel_ergebnisse':
    case 'view_spielplan':
    case 'panel_tabelle':
    case 'view_rangliste_teams_crosstable':
    case 'view_players_leaderboard':
	case 'view_rangliste_mannschaften_liga_2':
	case 'view_teams_leaderbord':
      $season = arg(1);
	  if(!arg(2)) $league = 517;
	  else $league=arg(2);
      break;
  }
 
  
  switch(arg(0)) {
  case "vereine-der-2-bundesliga-nord":
  $league=518;
  break;
  case "vereine-der-2-bundesliga-west":
  $league=519;
  break;
  case "vereine-der-2-bundesliga-ost":
  $league=520;
  break;
  case "vereine-der-2-bundesliga-sued":
  $league=521;
  break;
  case "vereine-der-bundesliga":
  $league=517;
  break;
  case "frauen-bundesliga":
  case "vereine-der-frauen-bundesliga":
  case "setzliste-frauen-bundesliga":
  $league=560;
  break;
  }
  
  switch($league) {
  case 517:
  $path_addon="";
  $league_addon="bundesliga";
  break;
  case 518:
  $path_addon="-liga-2";
  $league_addon="2-bundesliga-nord";
  break;
  case 519:
  $path_addon="-liga-2";
  $league_addon="2-bundesliga-west";
  break;
  case 520:
  $path_addon="-liga-2";
  $league_addon="2-bundesliga-ost";
  break;
  case 521:
  $path_addon="-liga-2";
  $league_addon="2-bundesliga-sued";
  break;
  case 560:
  $path_addon="-frauen-bundesliga";
  $league_addon="frauen-bundesliga";
  break;
  }

  
  
  $league_name=set_league_name($league);
  

  // Set the necessary variables for our theme_list() function
  $title = 'Tabellen ' . $league_name;
  $type = 'ul';
  $attributes = array('class' => array('tabelle', 'spielplan', 'ergebnisse','setzliste'));
  $list_items = array();
  $list_items[] = l('Tabelle', 'tabelle' . $path_addon . '/' . $season . '/' . $league);
  $list_items[] = l('Kreuztabelle', 'kreuztabelle' . $path_addon . '/' . $season . '/' . $league);
  $list_items[] = l('Spielplan', 'spielplan' . $path_addon . '/' . $season . '/' . $league);
  $list_items[] = l('Rundenergebnisse', 'ergebnisse/' . $season . '/' . $league . '/' . $matchday);
  $list_items[] = l('Teamstatistik', 'setzliste' . $path_addon . '/' . $season . '/' . $league);
  if($season==get_current_season()) $list_items[] = l('Vereine & Adressen', 'vereine-der-' . $league_addon);
  #$list_items[] = l('Topscorer', 'spieler-rangliste/' . $season . '/' . $league);

  // Create an unordered List with the navigation items
  $output = theme_item_list(array('items' => $list_items, 'title' => $title, 'type' => $type, 'attributes' => $attributes));
  return $output;
}

function set_league_name($league) {
 switch($league) {
  case 517:
  $league_name="1. Bundesliga";
  break;
  case 518:
  $league_name="2. Liga Nord";
  break;
  case 519:
  $league_name="2. Liga West";
  break;
  case 520:
  $league_name="2. Liga Ost";
  break;
  case 521:
  $league_name="2. Liga Süd";
  break;
  case 560:
  $league_name="Frauen-Bundesliga";
  break;
  default:
   $league_name="1. Bundesliga";
  }
  return $league_name;
}

function set_link_to_league_table($league) { // Legt Links für Seiten-Navi für Tabellenübersicht fest
$season=arg(2);

 switch($league) {
  case 518:
  case 519:
  case 520:
  case 521:
	$link="tabelle-liga-2/" . $season . "/" . $league;
  break;
  case 560:
	$link="tabelle-frauen-bundesliga/" . $season . "/" . $league;
	break;
   default:
   $link="tabelle/" . $season . "/" . $league;
  }
  return $link;
}

/**
 * Create a form that sets the season by changing the URL
 */
function select_season_form($form, &$form_state) {
  // Get the default value for our Season select
  $default_season = '';
  switch(statistics_check_type_of_page()) {
    case 'panel_ergebnisse':
    case 'view_spielplan':
    case 'panel_tabelle':
    case 'view_rangliste_teams_crosstable':
    case 'view_players_leaderboard':
	case 'view_teams_leaderbord':
      $default_season = arg(1);
      break;

  }

  if(arg(2) && strlen(arg(2)) > 2) $league=arg(2);
  else $league=517;
  
  if($league==517) {
  $form['set_season'] = array(
    '#type' => 'select',
    '#title' => 'Saison',
    '#options' => array(
	564 => '2018/19',
	562 => '2017/18',
	522 => '2016/17',
	144 => '2015/16',
	 143 => '2014/15',
	 142 => '2013/14',
	 139 => '2012/13',
	258 => '2011/12',
	261 => '2010/11',
	283 => '2009/10',
	284 => '2008/09',
	552 => '2007/08',
	553 => '2006/07',
	554 => '2005/06',
	555 => '2004/05',
	556 => '2003/04',
	557 => '2002/03',
	558 => '2001/02',
	),
    '#default_value' => $default_season,
    '#description' => 'Wählen Sie die Saison',
    //'#default_value' => variable_get('current_matchday', ''),
  );
  }
  else {
  $form['set_season'] = array(
    '#type' => 'select',
    '#title' => 'Saison',
    '#options' => array(
	564 => '2018/19',
	562 => '2017/18',
	),
    '#default_value' => $default_season,
    '#description' => 'Wählen Sie die Saison',
    //'#default_value' => variable_get('current_matchday', ''),
  );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  // Attach a Javascript file to make the form auto-submit when we change the value of the select element
  $form['#attached']['js'][] = drupal_get_path('module', 'sbl_helper') . '/js/sbl_helper_form_submit.js';

  return $form;
}
function select_season_form_validate($form, &$form_state) {
  // Set where to redirect, when we select the season

  // Get the season from our dropdown
  $season = $form_state['values']['set_season'];
  $url = '';
  //dpm(statistics_check_type_of_page());
  // Set the url to go to for each page
  switch(statistics_check_type_of_page()) {
    case 'panel_ergebnisse':
      $url = '/' . arg(0);
      $url .= '/' . $season;
	  $url .= '/517';
      $url .= '/' . arg(3);
      break;
    case 'view_spielplan':
    case 'panel_tabelle':
    case 'view_rangliste_teams_crosstable':
    case 'view_players_leaderboard':
	case 'view_teams_leaderbord':
      $url = '/' . arg(0);
      $url .= '/' . $season;
	  $url .= '/517';
      break;
  }
  // Set the form redirect to the above url
  $form_state['redirect'] = $url;
}

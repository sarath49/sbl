<?php

/**
 * Get all single games for a player for the current season.
 * Get some more values like the points won and the opponent's ELO.
 */

function player_get_games($player_team,$earlier_season='') {

  $player_nid = arg(1);
  if(strlen($earlier_season) > 0) $season=$earlier_season;
  else $season = arg(2);


  // Get all the team matches for the player's club
  // Get all home matches for the players team for the current season
  $player_season_home_matches_query = new EntityFieldQuery();

  $player_season_home_matches_query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'team_match')
    ->fieldCondition('field_tm_saison', 'tid', $season)
    ->fieldCondition('field_tm_team_1', 'target_id', $player_team)
    // Bypass node access
    ->addTag('DANGEROUS_ACCESS_CHECK_OPT_OUT');

  $player_season_home_matches = $player_season_home_matches_query->execute();

  // Get all away matches for the players team for the current season
  $player_season_away_matches_query = new EntityFieldQuery();

  $player_season_away_matches_query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'team_match')
    ->fieldCondition('field_tm_saison', 'tid', $season)
    ->fieldCondition('field_tm_team_2', 'target_id', $player_team)
    // Bypass node access
    ->addTag('DANGEROUS_ACCESS_CHECK_OPT_OUT');

  $player_season_away_matches = $player_season_away_matches_query->execute();

  // Write home and away games into one array
  $season_matches_combined = array();

  // Write the home games in to the $season_games_combined array
  if (isset($player_season_home_matches['node'])) {
    foreach ($player_season_home_matches['node'] as $season_home_match) {
      $season_matches_combined[] = $season_home_match->nid;
    }
  }

  // Write the away games in to the $season_games_combined array
  if (isset($player_season_away_matches['node'])) {
    foreach ($player_season_away_matches['node'] as $season_away_match) {
      $season_matches_combined[] = $season_away_match->nid;
    }
  }

  // Get all the single games the player was involved in
  // Get the home games for the player
  $player_season_home_games_query = new EntityFieldQuery();

  $player_season_home_games_query->entityCondition('entity_type', 'field_collection_item')
    ->entityCondition('bundle', 'field_tm_game')
    ->fieldCondition('field_mt_player_1', 'target_id', $player_nid)
    // Bypass node access
    ->addTag('DANGEROUS_ACCESS_CHECK_OPT_OUT');

  $player_season_home_games = $player_season_home_games_query->execute();

// Get the home games for the player
  $player_season_away_games_query = new EntityFieldQuery();

  $player_season_away_games_query->entityCondition('entity_type', 'field_collection_item')
    ->entityCondition('bundle', 'field_tm_game')
    ->fieldCondition('field_mt_player_2', 'target_id', $player_nid)
    // Bypass node access
    ->addTag('DANGEROUS_ACCESS_CHECK_OPT_OUT');

  $player_season_away_games = $player_season_away_games_query->execute();

// Write home and away games into one array
  $player_games_combined = array();

  if (isset($player_season_home_games['field_collection_item'])) {
    foreach ($player_season_home_games['field_collection_item'] as $player_home_game) {
      // Write the home games for the player into the array $player_games_combined
      $player_games_combined[] = $player_home_game->item_id;
    }
  }

  if (isset($player_season_away_games['field_collection_item'])) {
    foreach ($player_season_away_games['field_collection_item'] as $player_away_game) {
      $player_games_combined[] = $player_away_game->item_id;
    }
  }


$single_games_team_season = array();

// Extract all field_tm_game entity IDs from the team match nodes.
// Thus we get a list of all the field collection item ids that were played that season
if (!empty($season_matches_combined)) {
  foreach ($season_matches_combined as $season_match) {
    $season_matches_combined_wrapper = entity_metadata_wrapper('node', $season_match);
    if($season_matches_combined_wrapper->__isset('field_tm_game')) {
      $single_games = $season_matches_combined_wrapper->field_tm_game->value();
      foreach ($single_games as $game) {
        // Write the field collection item IDs into the array
        $single_games_team_season[] = $game->item_id;
      }
    }
  }
}

// Now get an intersection between the two queries: the entity_ids of the single games
// the player played - which are contained in the array $player_games_combined
// and all single games that were played by his team in the season - which are contained
// in the array $single_games_team_season. The field collection items that are in both arrays
// have been played by the player in the season

$single_games_player_season = array();
$single_games_player_season = array_intersect($player_games_combined, $single_games_team_season);

  $games_data = array();

foreach($single_games_player_season as $key => $game_id) {
  $game_wrapper = entity_metadata_wrapper('field_collection_item', $game_id);

  // Get the opponent's elo
  $opponent_elo = '';
  $opponent_name = '';
  $points_player = '';
  $player_elo = '';
  $league="";

  $player_1 = $game_wrapper->field_mt_player_1->value();
  $player_2 = $game_wrapper->field_mt_player_2->value();
  if(!empty($player_1) && !empty($player_2) && $game_wrapper->field_mt_player_1->nid->value() == $player_nid)  {
    // The points are the points for player 1
    $points_player = $game_wrapper->field_mt_points_player_1->label();
    // The opponent is player 2
    $opponent = $game_wrapper->field_mt_player_2->value();
    if(!empty($opponent)) {
      $opponent_name = $opponent->title;
      // Now iterate through the field collections field_sp_verein of the opponent and find the one
      // for the current season
      $season_data = $game_wrapper->field_mt_player_2->field_sp_verein->value();
      if (!empty($season_data)) {
        foreach ($game_wrapper->field_mt_player_2->field_sp_verein->value() as $season_data) {
          if (!empty($season_data->field_saison) && $season_data->field_saison[LANGUAGE_NONE][0]['tid'] == $season) {
            // The opponent elo is the one inside the field collection item for the current season
            $opponent_elo = $season_data->field_pl_elo[LANGUAGE_NONE][0]['value'];
			$league = $season_data->field_pl_league[LANGUAGE_NONE][0]['tid'];
          }
        }
		foreach ($game_wrapper->field_mt_player_1->field_sp_verein->value() as $season_data) {
          if (!empty($season_data->field_saison) && $season_data->field_saison[LANGUAGE_NONE][0]['tid'] == $season) {
            // The player's elo is the one inside the field collection item for the current season
            $player_elo = $season_data->field_pl_elo[LANGUAGE_NONE][0]['value'];
          }
        }
      }
    }
  } elseif(!empty($player_2) && !empty($player_1) && $game_wrapper->field_mt_player_2->nid->value() == $player_nid)  {
    // The points are the points for player 2
    $points_player = $game_wrapper->field_mt_points_player_2->label();
    $opponent = $game_wrapper->field_mt_player_1->value();
    if(!empty($opponent)) {
      $opponent_name = $opponent->title;
      // Now iterate through the field collections field_sp_verein and find the one
      // for the current season
      $season_data = $game_wrapper->field_mt_player_1->field_sp_verein->value();
      if (!empty($season_data)) {
        foreach ($game_wrapper->field_mt_player_1->field_sp_verein->value() as $season_data) {
          //dpm($season_data);
          if (!empty($season_data->field_saison) && $season_data->field_saison[LANGUAGE_NONE][0]['tid'] == $season) {
            $opponent_elo = $season_data->field_pl_elo[LANGUAGE_NONE][0]['value'];
			$league = $season_data->field_pl_league[LANGUAGE_NONE][0]['tid'];
          }
        }
		foreach ($game_wrapper->field_mt_player_2->field_sp_verein->value() as $season_data) {
          if (!empty($season_data->field_saison) && $season_data->field_saison[LANGUAGE_NONE][0]['tid'] == $season) {
            // The player's elo is the one inside the field collection item for the current season
            $player_elo = $season_data->field_pl_elo[LANGUAGE_NONE][0]['value'];
          }
        }
      }
    }
  }
  $games_data[$key]['game_id'] = $game_id;
  $games_data[$key]['points_player'] = $points_player;
  $games_data[$key]['opponent_name'] = $opponent_name;
  $games_data[$key]['opponent_elo'] = $opponent_elo;
  $games_data[$key]['player_elo'] = $player_elo;
  $games_data[$key]['season'] = $season;
}

return $games_data;

}

/*
 * Get the number of games a player has played in the current season
 */

function player_number_played_games($league) {
  $number_played_games = 'O';
  $player_games = drupal_static('player_games');

  if(!empty($player_games[$league])) {
    $number_played_games = count($player_games[$league]);
  }

  return $number_played_games;

}

/*
 * Get the number of games a player has played in specific season
 */

function player_number_played_games_per_season($saison) {
	
  $number_played_games = 'O';
  $player_games_per_season = drupal_static('player_games_all_seasons');

  if(!empty($player_games_per_season)) {
	if($saison != 0) $number_played_games = count($player_games_per_season[$saison]);
	else {
	$counter=0;
	foreach ($player_games_per_season as $spielzeit => $games) {
	$counter+=count($games);
	}
	$number_played_games=$counter;
  }

  }
 
  return $number_played_games;

}



/*
 * Get the average ELO of the opponents of the player
 */

function player_elo_opponents($league) {

  $games_player = drupal_static('player_games');
  $elo_sum = '';
  $unrated_players=0;

  foreach($games_player[$league] as $game) {
    if($game['opponent_elo'] > 0) $elo_sum += $game['opponent_elo'];
	else $unrated_players++;
	}

  $elo_total = '';

  if(!empty($games_player[$league])) {
    $count_games = count($games_player[$league]) - $unrated_players;
    $elo_total = round($elo_sum / $count_games);
  }

  //dpm($elo_total);

  return $elo_total;
}


/*
 * Get the average ELO of the opponents of the player in specific season
 */

function player_elo_opponents_all_seasons($season) {

  $games_player = drupal_static('player_games_all_seasons');
  $elo_sum = '';
  $elo_total = '';
  $count_games = '';
  $spielzeiten=array_keys($games_player);
  $unrated_players=0;
 
if($season !=0) {
  foreach($games_player[$season] as $game) {
     if($game['opponent_elo'] > 0) $elo_sum += $game['opponent_elo'];
	 else $unrated_players++;
  }
  

  if(!empty($games_player[$season])) {
    $count_games = count($games_player[$season]) - $unrated_players;
    $elo_total = round($elo_sum / $count_games);
  }
}
else {
	for($i=0;$i<count($spielzeiten);$i++) {
	$unrated_players=0;
	foreach($games_player[$spielzeiten[$i]] as $game) {
    if($game['opponent_elo'] > 0) $elo_sum += $game['opponent_elo'];
	 else $unrated_players++;
  }
 

  if(!empty($games_player[$spielzeiten[$i]])) {
    $count_games+= count($games_player[$spielzeiten[$i]]) - $unrated_players;
  }
	}
	if($count_games > 0) $elo_total = round($elo_sum / $count_games);
	else $elo_total=0;
}

  return $elo_total;
}



/*
 * Get the points a player has won in the current season
 */
function player_single_points($league) {

  $games_player = drupal_static('player_games');
  $points_sum = 0;

  foreach($games_player[$league] as $game) {
    $points_sum += $game['points_player'];
  }
  if(empty($points_sum)) {
    $points_sum = 'O';
  }

  return $points_sum;
}

/*
 * Get the points a player has won in specific season
 */

function player_single_points_all_seasons($season) {

  $games_player = drupal_static('player_games_all_seasons');
  $points_sum = 0;
  $spielzeiten=array_keys($games_player);

  if($season !=0) {
  foreach($games_player[$season] as $game) {
    $points_sum += $game['points_player'];
  }
  if(empty($points_sum)) {
    $points_sum = 'O';
  }
  }
  else {
	 for($i=0;$i<count($spielzeiten);$i++) {
	foreach($games_player[$spielzeiten[$i]] as $game) {
    $points_sum += $game['points_player'];
		}
	}
  if(empty($points_sum)) {
    $points_sum = 'O';
  }
  }

  return $points_sum;
}

/* Gets the number of unrated opponents in current season */

function player_unrated_opponents($league) {
$unrated_players=0;
$games_player = drupal_static('player_games');
	foreach($games_player[$league] as $game) {
		if($game['opponent_elo'] == 0) $unrated_players++;
	}
return $unrated_players;
}

/* Gets the number of points scored against unrated opponents in current season */

function player_points_versus_unrated_opponents($league) {
$scored_points=0;
$games_player = drupal_static('player_games');
	foreach($games_player[$league] as $game) {
		if($game['opponent_elo'] == 0) $scored_points+=$game['points_player'];
	}
return $scored_points;
}



/* Gets the number of unrated opponents in all seasons */

function player_unrated_opponents_all_seasons($season) {
$unrated_players=0;
$games_player = drupal_static('player_games_all_seasons');
$spielzeiten=array_keys($games_player);
	 if($season !=0) {
		foreach($games_player[$season] as $game) {
			if($game['opponent_elo'] == 0) $unrated_players++;
			}
		}
	else {
		for($i=0;$i<count($spielzeiten);$i++) {
			foreach($games_player[$spielzeiten[$i]] as $game) {
				if($game['opponent_elo'] == 0) $unrated_players++;
			}
		}
	}
return $unrated_players;
}

/* Gets the number of points scored against unrated opponents in all seasons */

function player_points_versus_unrated_opponents_all_seasons($season) {
$scored_points=0;
$games_player = drupal_static('player_games_all_seasons');
$spielzeiten=array_keys($games_player);
	if($season !=0) {
		foreach($games_player[$season] as $game) {
			if($game['opponent_elo'] == 0) $scored_points+=$game['points_player'];
		}
	}
	else {
		for($i=0;$i<count($spielzeiten);$i++) {
			foreach($games_player[$spielzeiten[$i]] as $game) {
				if($game['opponent_elo'] == 0) $scored_points+=$game['points_player'];
			}
		}
	}
return $scored_points;
}


/* ELO-Change for Player in current season */

function player_elo_change($league) {

$elo_player=0;
$games_player = drupal_static('player_games');
  foreach($games_player[$league] as $game) {
    $elo_player = $game['player_elo'];
  }
  
  if($elo_player > 0 && !empty ($elo_player)) {
  $kfaktor=$elo_player >=2400?10:20;
  $games_played=player_number_played_games($league) - player_unrated_opponents($league);
  $eloschnitt=player_elo_opponents($league);
  $elo_summe=$games_played * $eloschnitt;
  $points_scored=player_single_points($league) - player_points_versus_unrated_opponents($league);
  $exponent=($eloschnitt - $elo_player) / 400;
  $we=1/(1+(pow(10,$exponent)));
 
  $summe_gewinnerwartungen=round(($we * $games_played),3);
  $elo_new=$elo_player + ($kfaktor *($points_scored-$summe_gewinnerwartungen));
  $difference=round($elo_new-$elo_player,1);
  $change=$difference;
  }
  else $change=0;

  return $change;
}

/* ELO-Change for Player in specific season */

function player_elo_change_all_seasons($season) {

$elo_player=0;
$games_player = drupal_static('player_games_all_seasons');
$spielzeiten=array_keys($games_player);

if($season !=0) {
  foreach($games_player[$season] as $game) {
    $elo_player = $game['player_elo'];
  }
  
  if($elo_player > 0 && !empty ($elo_player)) {
  $kfaktor=$elo_player >=2400?10:20;
  $games_played=player_number_played_games_per_season($season) - player_unrated_opponents_all_seasons($season);
  $eloschnitt=player_elo_opponents_all_seasons($season);
  $elo_summe=$games_played * $eloschnitt;
  $points_scored=player_single_points_all_seasons($season) - player_points_versus_unrated_opponents_all_seasons($season);
  $exponent=($eloschnitt - $elo_player) / 400;
  $we=1/(1+(pow(10,$exponent)));
 
  $summe_gewinnerwartungen=round(($we * $games_played),3);
  $elo_new=$elo_player + ($kfaktor *($points_scored-$summe_gewinnerwartungen));
  $difference=round($elo_new-$elo_player,1);
  $change=$difference > 0?"<span style='color:green'>+".$difference."</span>":"<span style='color:red'>".$difference."</span>";
  }
  else $change=0;
}

else {
	$change_temp=0;
for($i=0;$i<count($spielzeiten);$i++) {
	foreach($games_player[$spielzeiten[$i]] as $game) {
    $elo_player = $game['player_elo'];
	}
	if($elo_player > 0 && !empty ($elo_player)) {
	$kfaktor[$i]=$elo_player >=2400?10:20;
	$games_played[$i]=player_number_played_games_per_season($spielzeiten[$i]) - player_unrated_opponents_all_seasons($spielzeiten[$i]);
	$eloschnitt[$i]=player_elo_opponents_all_seasons($spielzeiten[$i]);
	$elo_summe[$i]=$games_played[$i] * $eloschnitt[$i];
	$points_scored[$i]=player_single_points_all_seasons($spielzeiten[$i]) - player_points_versus_unrated_opponents_all_seasons($spielzeiten[$i]);
	$exponent[$i]=($eloschnitt[$i] - $elo_player) / 400;
	$we[$i]=1/(1+(pow(10,$exponent[$i])));
 
	$summe_gewinnerwartungen[$i]=round(($we[$i] * $games_played[$i]),3);
	$elo_new[$i]=$elo_player + ($kfaktor[$i] *($points_scored[$i]-$summe_gewinnerwartungen[$i]));
	$difference[$i]=round($elo_new[$i]-$elo_player,1);
	$change_temp+=$difference[$i];
	}
	
	else $change=0;
	}
	$change=$change_temp > 0?"<span style='color:green'>+".$change_temp."</span>":"<span style='color:red'>".$change_temp."</span>";
}

  return $change;
}

function update_elo($club,$elo_change) {
$elo_change=str_replace("<span style='color:","",$elo_change);
$elo_change=str_replace("</span>","",$elo_change);
$elo_change=str_replace("green'>","",$elo_change);
$elo_change=str_replace("red'>","",$elo_change);
$elo_change=str_replace("+","",$elo_change);
$elo_change=floatval($elo_change);
//dpm($elo_change);
$player=intval(arg(1));
$season=intval(arg(2));
$club=intval($club);
$elo_change=doubleval($elo_change);
$checkquery = db_query("SELECT * FROM sbl_elo_gain_per_team WHERE season=".$season." AND team=".$club." AND player=".$player." LIMIT 1")->fetchField();
if(empty($checkquery)) $insertquery=db_query("INSERT INTO sbl_elo_gain_per_team(season,team,player,elo_change) values($season,$club,$player,$elo_change)");
else $updatequery=db_query("UPDATE sbl_elo_gain_per_team SET elo_change=$elo_change WHERE season=".$season." AND team=".$club." AND player=".$player);
}

/*
 * Calculate the percentage of wins for the player for the current season
 */
function player_single_percentage($league) {
  $percentage = 'O';

  $games_player = drupal_static('player_games');

  $points_sum = '';

  foreach($games_player[$league] as $game) {
    $points_sum += $game['points_player'];
  }

  $number_games = '';
  $number_games = count($games_player[$league]);
  if($number_games != '' && $points_sum != '') {
    $percentage = round(($points_sum / $number_games) * 100);
  }
  return $percentage;
}

/*
 * Calculate the percentage of wins for the player a specific season
 */
function player_single_percentage_all_seasons($season) {
  $percentage = 'O';

  $games_player = drupal_static('player_games_all_seasons');

  $points_sum = '';

  foreach($games_player[$season] as $game) {
    $points_sum += $game['points_player'];
  }

  $number_games = '';
  $number_games = count($games_player[$season]);
  if($number_games != '' && $points_sum != '') {
    $percentage = round(($points_sum / $number_games) * 100);
  }
  return $percentage;
}




/*
 * Calculate the Elo Performance (simplified formula) for the player for
 * the current season.
 */
function player_single_elo_performance($league) {
  $games_player = drupal_static('player_games');
  $elo_performance = '';
  $elo_sum = '';
  $elo_game = '';
  $unrated_players=0;

  foreach($games_player[$league] as $game) {
    $elo_game = $game['opponent_elo'];
	if($elo_game > 0) {
    if($game['points_player'] == 1) {
      $elo_sum += $elo_game + 400;
    } elseif($game['points_player'] == 0) {
      $elo_sum += $elo_game - 400;
    } elseif($game['points_player'] == 0.5) {
      $elo_sum += $elo_game;
    }
	}
	else $unrated_players++;
  }
  if(!empty($games_player[$league])) {
    $count_games = count($games_player[$league]) - $unrated_players;
    $elo_performance = round($elo_sum / $count_games);
  }
  return $elo_performance;
}

/*
 * Calculate the Elo Performance (simplified formula) for the player for
 * a specific season.
 */
function player_single_elo_performance_all_seasons($season) {
  $games_player = drupal_static('player_games_all_seasons');
  $elo_performance = '';
  $elo_sum = '';
  $elo_game = '';
  $count_games = '';
  $unrated_players=0;
  $spielzeiten=array_keys($games_player);

  if($season !=0) {
  foreach($games_player[$season] as $game) {
    $elo_game = $game['opponent_elo'];
	if($elo_game > 0) {
    if($game['points_player'] == 1) {
      $elo_sum += $elo_game + 400;
    } elseif($game['points_player'] == 0) {
      $elo_sum += $elo_game - 400;
    } elseif($game['points_player'] == 0.5) {
      $elo_sum += $elo_game;
    }
	}
	else $unrated_players++;
  }
  if(!empty($games_player[$season])) {
    $count_games = count($games_player[$season]) - $unrated_players;
    $elo_performance = round($elo_sum / $count_games);
  }
  }
  else {
	for($i=0;$i<count($spielzeiten);$i++) {
	$unrated_players=0;
	foreach($games_player[$spielzeiten[$i]] as $game) {
    $elo_game = $game['opponent_elo'];
	if($elo_game > 0) {
    if($game['points_player'] == 1) {
      $elo_sum += $elo_game + 400;
    } elseif($game['points_player'] == 0) {
      $elo_sum += $elo_game - 400;
    } elseif($game['points_player'] == 0.5) {
      $elo_sum += $elo_game;
    }
	}
	else $unrated_players++;
	}
	 if(!empty($games_player[$spielzeiten[$i]])) {
    $count_games += count($games_player[$spielzeiten[$i]]) - $unrated_players;
	}	
			  
  }
  if($count_games > 0) $elo_performance = round($elo_sum / $count_games);
  else $elo_performance=0;
  }
  

  return $elo_performance;
}

/* Gets row number for historic player's result */

function get_view_rowcount(){

$view = views_get_current_view();
$page_total = count($view->result);
if(isset($view->total_rows)){
return $view->total_rows;
} else {
return $page_total;
}
}

/* Board points gained by a team in a particular season */

function get_board_points($season) {
$team=arg(1);
$league = db_query("SELECT league FROM sbl_team_points WHERE node_id=".$team." AND season=".$season)->fetchField();
$board_points = db_query("SELECT board_points FROM sbl_team_points WHERE node_id=".$team." AND season=".$season)->fetchField();

switch($league) {
case 517: 
return $board_points . ":".number_format((120-$board_points),1,'.','');
break;
case 560: 
return $board_points . ":".number_format((66-$board_points),1,'.','');
break;
default:
return $board_points . ":".number_format((72-$board_points),1,'.','');
break;
}

}

/* Teamn points gained by a team in a particular season */

function get_team_points($season) {
$team=arg(1);
$league = db_query("SELECT league FROM sbl_team_points WHERE node_id=".$team." AND season=".$season)->fetchField();
$team_points = db_query("SELECT team_points FROM sbl_team_points WHERE node_id=".$team." AND season=".$season)->fetchField();
switch($league) {
case 517: 
return $team_points . ":".(30-$team_points);
break;
case 560: 
return $team_points . ":".(22-$team_points);
break;
default:
return $team_points . ":".(18-$team_points);
break;
}
}

/* Returns the ranking position in a particular season */

function get_team_ranking($season) {
$team=arg(1);
$league = db_query("SELECT league FROM sbl_team_points WHERE node_id=".$team." AND season=".$season)->fetchField();
$all_teams_per_league = db_query("SELECT * FROM sbl_team_points WHERE season=".$season." AND league=".$league." ORDER BY team_points DESC, board_points DESC")->fetchAll();
$count=1;
foreach ($all_teams_per_league as $record) {
	if($record->node_id==$team) return $count . ".";
	else $count++;
}
}

/* Returns the status of each team in a particular season */
function get_team_status($season,$team,$league) {
$all_teams_per_league = db_query("SELECT * FROM sbl_team_points WHERE season=".$season." AND league=".$league." ORDER BY team_points DESC, board_points DESC")->fetchAll();
$platz="";
$count=1;
foreach ($all_teams_per_league as $record) {
	if($record->node_id==$team) $platz=$count;
	else $count++;
}
switch($platz) {
	case 1:
	$title="<strong>Amtierender Meister</strong>";
	break;
	case "":
	$title="<span style='color:green;font-weight:bold'>Aufsteiger</span>";
	break;
	default:
	$title="Vorsaison: ".$platz.". Platz";
	break;
}
return $title;
}

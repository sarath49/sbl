<?php
// Link the title to the season that we get from the tid in the first argument
// in the url and the nid which is the Verein Node ID
$season = get_current_season();
$path = 'verein/' . $row->nid . '/' . $season;
//dpm($row);
$output = l($row->node_title, $path);
?>

<?php print $output; ?>
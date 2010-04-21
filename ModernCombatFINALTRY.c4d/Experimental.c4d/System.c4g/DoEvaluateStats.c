/*-- Statevaluation --*/

#strict 2


global func DoEvaluateStats()
{
  //Das Regelobjekt
  var database = FindObject(AR_A);

  //Für jeden Spieler
  for(var i=0; i < database->GetPlayerAmount(); i++)
  {
   AddEvaluationData(Format("$Complete$",
                            database->GetPlayerStats("Name", i),
                            database->GetPlayerStats("Killpoints", i),
                            database->GetPlayerStats("Teampoints", i),
                            database->GetPlayerStats("Negativepoints", i)),0);
   AddEvaluationData(
     Format("$CompletePoints$",
            database->GetPlayerStats("Name", i),
            database->GetPlayerStats("Totalpoints", i)),0);
  }
}

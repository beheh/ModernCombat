/*-- Statevaluation --*/

#strict 2


global func DoEvaluateStats()
{
  //Das Regelobjekt
  var databank = FindObject(AR_A);

  //Für jeden Spieler
  for(var i=0; i < databank->GetPlayerAmount(); i++)
  {
   var plr = GetPlayerByIndex(i);
   AddEvaluationData(Format("$Complete$",
                            databank->GetPlayerStats("Name", i),
                            databank->GetPlayerStats("Killpoints", i),
                            databank->GetPlayerStats("Teampoints", i),
                            databank->GetPlayerStats("Negativepoints", i)),GetPlayerID(plr)+1);
   AddEvaluationData(
     Format("$CompletePoints$",
            databank->GetPlayerStats("Name", i),
            databank->GetPlayerStats("Totalpoints", i)),0);
  }
}
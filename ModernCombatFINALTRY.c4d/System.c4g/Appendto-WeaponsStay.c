/*-- Waffen bleiben --*/

//Waffen bleiben-Regel aktualisiert die Sichtbarkeit von respawnenden Items für beitretende Spieler.

#strict 2
#appendto WPST


protected func InitializePlayer(int iPlr)
{  
  for(var spwnpnt in FindObjects(Find_ID(SPNP)))
    if(GetPlayerType(GetPlayerByIndex(iPlr)))
      if(!(Local(0,spwnpnt) & 1<<GetPlayerByIndex(iPlr)))
        Local(0,spwnpnt) += 1<<GetPlayerByIndex(iPlr);
  return(1);
}
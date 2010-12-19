/*-- Capture the Flag --*/

//Capture the Flag erstellt bei Spielstart Spielzielhinweise.

#strict 2

#appendto GCTF


public func ChooserFinished()
{
  //Spielzielhinweise erstellen
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    DoScoreboardShow(1, GetPlayerByIndex(i)+1);
    CreateObject(TK06, 0, 0, GetPlayerByIndex(i));
    Sound("RadioConfirm*.ogg", true, 0, 100, GetPlayerByIndex(i));
  }
}
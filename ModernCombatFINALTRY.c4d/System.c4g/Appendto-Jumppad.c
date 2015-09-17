/*--  Sprungschanze --*/

//Jumpad-Kompatibilität mit Fallschirmen und Wandsprung.

#strict 2
#appendto JPTP


private func Bounce(object pClonk)
{
  var pParachute = FindObject2(Find_ID(PARA), Find_ActionTarget(pClonk));

  //Fallschirm schließen sofern vorhanden
  if(pParachute)
    pParachute->Close();

  //Neuen Fallschirm erlauben
  RemoveEffect("Flying", pClonk);

  //Eventuell machbaren Wandsprung verhindern
  RemoveEffect("IntWallJump", pClonk);

  return inherited(pClonk);
}
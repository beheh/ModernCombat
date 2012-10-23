/*--  Jumpad-Kompatibilit‰t mit Fallschirmen und Wandsprung --*/

#strict 2

#appendto JPTP

private func Bounce(object pClonk) {
  var pParachute = FindObject2(Find_ID(PARA), Find_ActionTarget(pClonk));
  if(pParachute) pParachute->Close(); //alten Fallschirm schlieﬂen
  RemoveEffect("Flying", pClonk); //neuen Fallschirm erlauben
  RemoveEffect("IntWallJump", pClonk); //Wandsprung verhindern
  return inherited(pClonk);
}

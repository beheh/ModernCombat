/*-- Neues Script --*/

#strict 2
#appendto GSBO

private func InstaExplode(bool noRemove)
{
  // Effekte
  CastParticles("Wave", 7+Random(5), RandomX(35,55), 0,0, 20,40, RGBa(150,150,200,50), RGBa(150,150,255,100));
  AddLightFlash(60, 0,0, RGBa(255,255,200,100));
  Sparks(7+Random(5), RGBa(255,255,150,100));
  // Explosion
  FakeExplode(25+Random(20));
  if(noRemove) return(1);
  // Weggehen
  RemoveObject();
}

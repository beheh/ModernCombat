/*--  --*/


#strict 2
#appendto OFPL

/* Flaggenposten verschieben */

public func MoveFlagpost(int iX, int iY, bool fNeutral, string szName)
{
  //Effekte
  for(var i = -80; i < -20; i += 10)
    CastParticles("MetalSplinter",1,20,0,i,50,80);
  if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",8,15,0,-5,250,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
  CastParticles("Sandbag", 10, 70, 0,-10, 35, 45, RGBa(228,228,228,0), RGBa(250,250,250,50));
  Sound("FenceDestruct.ogg");
  Sound("StructureHit*.ogg");
  Sound("StructureDebris*.ogg");

  //Namen ändern
  if(szName)
    SetName(szName);

  //Besitzer neutralisieren
  if(fNeutral)
    NoTeam();

  //Spawnpunkte verschieben
  var curX = GetX(), curY = GetY();
  for(var i = 0; i < GetLength(spawnpoints); i++)
  {
    spawnpoints[i][0] -= iX - curX;
    spawnpoints[i][1] -= iY - curY;
  }

  //Verschieben und einblenden
  SetPosition(iX, iY);
  UpdateFlag();
  FadeIn();
  flag->FadeIn();
}
/*-- Maststück --*/

#strict 2


/* Initalisierung */

func Initialize()
{
  SetAction("Be");

  //Verschwinden planen
  Schedule("FadeOut()", RandomX(800,1000));
}

/* Aufschlag */

public func Hit(int iXDir, int iYDir)
{
  //Spielerbildschirme schütteln
  ShakeViewPort(50, this);

  //Effekte
  if(GetEffectData(EFSM_ExplosionEffects) > 1)
  {
    var x = GetX(), y = GetY(), xdir, ydir;
    SimFlight(x, y, xdir, ydir, 50, 100);
    var mat = GetMaterial(x, y);

    if(mat == Material("Vehicle")) mat = Material("Wall");
    var clr_offset = Random(3)*3;
    var clr = RGB(Min(GetMaterialVal("Color", "Material", mat, 0 + clr_offset), 255), 
    		Min(GetMaterialVal("Color", "Material", mat, 1 + clr_offset), 255), 
    		Min(GetMaterialVal("Color", "Material", mat, 2 + clr_offset), 255));

    CastParticles("GroundSmoke", RandomX(4,12), 80, AbsX(x), AbsY(y)-1, (iXDir + iYDir) / 4, (iXDir + iYDir) / 2, clr, clr);
  }
  CastSmoke("Smoke",4,10,0,0,500,100,RGBa(255,255,255,0));
  Sound("StructureHit*.ogg");

  return true;
}
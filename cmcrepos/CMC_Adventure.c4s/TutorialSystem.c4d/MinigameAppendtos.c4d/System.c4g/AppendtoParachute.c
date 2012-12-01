/*-- Neues Script --*

#strict 2

#appendto PARA

public func Set(object pObj)
{
	LocalN("parachute", pObj) = this;
	AddEffect("DmgClosing", pObj, 101, 1, this);
  _inherited(pObj, ...);
}

public func FxDmgClosingDamage(object target, int nr, int dmgengy)
{
	LocalN("paracute", target)->SetAction("StartFlyFree");
	return dmgengy;
}

public func ControlLeft()
{
	SetXDir(-10, GetActionTarget());
	return true;
}

public func ControlRight()
{
	SetXDir(10, GetActionTarget());
	return true;
}

protected func Fly()
{
  //Freiflug
  if(WildcardMatch(GetAction(),"*Free*"))
  {
    //Bei baldigem Bodenkontakt zusammenfallen
    if(GBackSolid(0, 20))
      Close();
    //Windbeeinflussung
    SetXDir(GetWind(GetX(), GetY()) / 8);
    SetYDir(30, 0, 10);
    //In Wasser zusammenfallen
    if(GetContact(0,-1, CNAT_Bottom) || InLiquid())
    {
      SetAction("FoldFree");
      Sound("ParachuteClose.ogg");
    }
  }
  //Mit Objekt
  else
  {
    var targ = GetActionTarget();
    if (!targ)
    {
      SetAction("StartFlyFree");
      return;
    }

    //Bei baldigem Bodenkontakt zusammenfallen
    if (GBackSolid(0, 20))
    {
      //Zusatzeffekte bei Objekteanhang
      if(targ)
      {
        Sound("ParachuteLand.ogg");
        if(GetEffectData(EFSM_ExplosionEffects) > 0) CastSmoke("Smoke3",8,10,0,20,20,150);
      }
      Close();
    }

    //Bei Wasserkontakt zusammenfallen
    if(InLiquid() || InLiquid(targ))
      Close();

    //Windbeeinflussung
    if(GetID(g_TutorialObject) != MBJP)
   		SetXDir(GetWind(GetX(), GetY()) /8 , targ);

    //Fall verlangsamen
    var y = Interpolate2(GetYDir(targ, 100), 300, 1, 5);
    SetYDir(y, targ, 100);

    //Manuelles Losmachen durch [Doppelstop] des Objektanhangs
    if(GetPlrDownDouble(GetController(targ)))
    {
      //Erneuter Check nach Bodenkontakt
      if(GBackSolid(0, 20))
        Close();
      else
        SetAction("StartFlyFree");
    }
    //Lebewesen
    if(GetCategory(targ) & C4D_Living)
      if(GetID(Contained(targ)) == FKDT)
      {
        SetAction("StartFlyFree");
        SetPosition(GetX(), GetY()+22); 
      }
      else
        if(GetID(targ) != FKDT && GetProcedure(targ) != "FLOAT" && GetProcedure(targ) != "FLIGHT")
          Close();

    if(Contained())
    {
      Exit();
      SetAction("Fly", targ, 0, true);
    }
  }
}*/

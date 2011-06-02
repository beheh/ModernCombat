/*-- Fallschirm --*/

#strict 2


/* Erstellung */

public func ControlDigDouble(pObj)
{
  Set(pObj);
}

public func Set(object pObj)
{
  Sound("ParachuteOpen*.ogg");
  SetAction("Open", pObj);
}

/* Timer */

protected func Fly()
{
  //Freiflug
  if(WildcardMatch(GetAction(),"*Free*"))
  {
    //Bei baldigem Bodenkontakt zusammenfallen
    if (GBackSolid(0, 20))
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
    SetXDir(GetWind(GetX(), GetY()) /8 , targ);

    //Fall verlangsamen
    var y = Interpolate2(GetYDir(targ, 100), 300, 1, 5);
    SetYDir(y, targ, 100);

    //Manuelles Losmachen durch [Doppelstop] des Objektanhangs
    if(GetPlrDownDouble(GetController(targ)))
    {
      //Erneuter Check nach Bodenkontakt
      if (GBackSolid(0, 20))
        Close();
      else
        SetAction("StartFlyFree");
    }
    //Lebewesen
    if (GetCategory(targ) & C4D_Living)
      if (GetID(Contained(targ)) == FKDT)
      {
        SetAction("StartFlyFree");
        SetPosition (GetX(), GetY()+22); 
      }
      else
        if (GetID(targ) != FKDT && GetProcedure(targ) != "FLOAT" && GetProcedure(targ) != "FLIGHT")
          Close();

    if (Contained())
    {
      Exit();
      SetAction("Fly", targ, 0, true);
    }
  }
}

/* Auf- und einklappen */

protected func Opening()
{
  if(GetActTime() > 25)
  {
    SetObjDrawTransform(1000, 0, 0, 0, 1000, 0, this);
    SetAction("Fly", GetActionTarget());
    return;
  }
  var w = Sin(36 * GetActTime() / 10, 1000);
  var h = 40 * GetActTime();

  SetObjDrawTransform(w, 0, 0, 0, h, InvertA1(500 * GetObjHeight() * h / 1000, 1000 * GetObjHeight()) - 500 * GetObjHeight(), this);
}

public func Close()
{
  SetAction("Fold", GetActionTarget());
  Sound("ParachuteClose.ogg");
}

private func Folded()
{
  //Verschwinden
  FadeOut();
}
/* HUD */

#strict

local pArrowR, //zeigt Rotation
      pArrowT, //zeigt Thrust
      pArrowF, //zeigt Fuel
      pArrowW; //zeigt Wind

local heli;

local fRed, Color;

protected func Initialize() 
{
  //leicht durchsichtig machen
  SetClrModulation(RGBa(0,200,0,50), this());
  //standard: grün
  fRed = false;
  Color = "green";
  return(true);
}

func DamageRecieved()
{
  AddEffect("DRecieved", this(), 100, 1, this());
  return(true);
}

protected func TimerFunc()
{
  //ohne Heli entfernen
  if (!heli) return(RemoveObject());
  if (!(LocalN("Pilot1", heli) || LocalN("Pilot2", heli))) return(RemoveObject());
  //Sichtbarkeit setzen
  SetVisibility(VIS_Local);
  if (LocalN("Pilot1", heli))
  {
    if (LocalN("Pilot1", heli) == GetCursor(GetOwner(LocalN("Pilot1", heli))))
      Local(0) = 2**GetOwner(LocalN("Pilot1", heli));
    else
      Local(0) = 0;
  }
  else
    Local(0) = 0;
  if (LocalN("Pilot2", heli))
  {
    if (LocalN("Pilot2", heli) == GetCursor(GetOwner(LocalN("Pilot2", heli))))
      Local(0) = Local(0) + 2**GetOwner(LocalN("Pilot2", heli));
  }
  //Position an Heli anpassen
  SetPosition(BoundBy(GetX(heli), 185, LandscapeWidth()-186),
              BoundBy(GetY(heli),  90, LandscapeHeight()-91), this());
  //Farbe bei Schaden anpassen
  if ((fRed || LocalN("fuel", heli) < 20000) && Color eq "green")
  {
    SetClrModulation(RGBa(255,0,0,50), this());
    Sound("DamageWarning");
    Color = "red";
  }
  if (!(fRed || LocalN("fuel", heli) < 20000) && Color eq "red")
  {
    SetClrModulation(RGBa(0,200,0,50), this());
    Color = "green";
  }
  //Rotation anzeigen
  if (!pArrowR)
  {
    pArrowR = CreateObject(H_H1,0,0,GetOwner());
    LocalN("hud",pArrowR) = this();
  }
  else
  {
    SetPosition(GetX(), GetY()-20, pArrowR);
    pArrowR->SetR(GetR(heli)*3/2);
  }
  //Schub anzeigen
  if (!pArrowT)
  {
    pArrowT = CreateObject(H_H2,0,0,GetOwner());
    LocalN("hud",pArrowT) = this();
  }
  else
  {
    SetPosition(GetX()-176,GetY()+21-(LocalN("throttle", heli)/2), pArrowT);
  }
  //Tankstand anzeigen
  if (!pArrowF)
  {
    pArrowF = CreateObject(H_H2,0,0,GetOwner());
    LocalN("hud",pArrowF) = this();
  }
  else
  {
    pArrowF -> SetPosition(GetX()+168,GetY()+56-LocalN("fuel", heli)/750);
    pArrowF -> SetR(180);
    //Bei zu wenig rot blinken
    if (LocalN("fuel", heli) < 20000 && GetAction(pArrowF) ne "Blink")
    {
      pArrowF -> SetAction("Blink");
    }
    if (LocalN("fuel", heli) > 20000 && GetAction(pArrowF) eq "Blink")
    {
      pArrowF -> SetAction();
    }
  }
  //Windrichtung anzeigen
  if (!pArrowW)
  {
    pArrowW = CreateObject(H_H2,0,0,GetOwner());
    LocalN("hud",pArrowW) = this();
  }
  else
  {
    SetPosition(GetX() - 131 + (GetWind(GetX(heli), GetY(heli)-20, false)*2)/5, GetY()+50, pArrowW);
    SetR(90, pArrowW);
  }

  return(true);
}

/* ----- Effekt: DRecieved ----- */

protected func FxDRecievedEffect(string NewEffectName, object Target, int EffectNumber, int NewEffectNumber, Var1, Var2, Var3, Var4)
{
  if (NewEffectName eq "DRecieved")
  {
    EffectVar(0, Target, EffectNumber) = 15;
    return(-1);
  }
}

protected func FxDRecievedStart(object Target, int EffectNumber, int Temp, Var1, Var2, Var3, Var4)
{
  EffectVar(0, Target, EffectNumber) = 15;
  LocalN("fRed", Target) = true;
  return(true);
}

protected func FxDRecievedStop(object Target, int EffectNumber, int Reason, bool Temp)
{
  LocalN("fRed", Target) = false;
  return(true);
}

protected func FxDRecievedTimer(object Target, int EffectNumber, int EffectTime)
{
  EffectVar(0, Target, EffectNumber)--;
  if (EffectVar(0, Target, EffectNumber) <= 0)
    return(-1);
}


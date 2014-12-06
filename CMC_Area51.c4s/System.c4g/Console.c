/*-- Konsole - überarbeitet, unterstützt nun mehrere Zielobjekte --*/

#strict 2
#appendto CONS

local aTargets; 
local aGroups;

protected func Initialize()
{
  aTargets = CreateArray();
  aGroups = CreateArray();
  return _inherited(...);
}

public func IsFakeRepairable(int iPlr)		
{
  for(var i = 0; i < GetLength(aTargets); i++)
    if(aTargets[i] && aTargets[i]->~IsRepairable() && ( GetDamage(aTargets[i]) || Hostile(iPlr, GetOwner(aTargets[i])) ) )
    {
	  target = aTargets[i];
	  return true;
	}
}

public func GetViewTarget()
{
  for(var i = 0; i < GetLength(aTargets); i++)
	if(aTargets[i])
	  return aTargets[i];
	  
  return 0;
}

/* Timer */

protected func Timer(pClonk)
{
  //Blinkeffekt
  AddLightFlash(100,0,0,RGB(0,0,255));
  CreateParticle("NoGravSpark", -8, 1, 0, 0, 50, RGBa(0, 0, 255, 50));
  CreateParticle("NoGravSpark", 8, 1, 0, 0, 50, RGBa(0, 0, 255, 50));
}


/* Bedienung */

protected func ControlDig(pClonk)
{
  [$Control$]

  var pTarget = GetViewTarget();  
  
  //Kein Menü wenn kein Ziel
  if(!pTarget) 
    return;

  //Sicht auf Zielobjekt zentrieren
  SetPlrView(GetController(pClonk),pTarget);

  //Menü erstellen
  CreateMenu(GetID(pTarget), pClonk, 0, 0, Format("$Control$: %s", GetName(pTarget)), 0, 1);
  
  var aHandledGroups = CreateArray();
  for(var k = 0; k < GetLength(aTargets); k++)
    if(GetIndexOf(aGroups[k],aHandledGroups) == -1 ) 
	{
      for(var i = 1, desc ; desc = aTargets[k]->~ConsoleControl(i, pClonk, this) ; i++)
      {
	    if(aGroups[k])
          var s = Format("$%s$",aGroups[k]); 
		else
		  var s = GetName(aTargets[i]);
		  
        AddMenuItem(Format("%s: %s", s, desc), Format("ConsoleControlled(%d, %d, %d, \"%s\")", i, ObjectNumber(pClonk), ObjectNumber(this), aGroups[k]), GetID(), pClonk, 0, 0, "$Control$");
        Sound("Acknowledge.ogg", 0, pClonk, 100, GetOwner(pClonk)+1);
      }
	  aHandledGroups[GetLength(aHandledGroups)] = aGroups[k];
	}
	
  return 1;
}

public func ConsoleControlled(int i1, i2, i3, string i4)
{
  //existiert nur für Abwärtskompatibilität!
  if(i4 != "")
  {  
    aTargets[0]->~ConsoleControlled(i1,i2,i3);	  
	return 1;
  }
  
  for(var k = 0; k < GetLength(aTargets); k++)
    if(aGroups[k] == i4)
      aTargets[k]->~ConsoleControlled(i1,i2,i3);
	  
  return 1;
}

/* Ziel setzen */

public func Set(pTarget, sGroup)
{
  if(!pTarget)
    return;
	
  aTargets[GetLength(aTargets)]=pTarget;
  aGroups[GetLength(aGroups)]=sGroup;
	  
  return this;
}

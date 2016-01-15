/*-- Info-Arrow --*/

#strict 2

public func MaxRange()	{return 1600;}
public func NoWarp()	{return true;}
public func IsHUD()	{return true;}

local arrowIcon;

//Informationsgrafik und Pfeilgrafik müssen getrennt sein, ansonsten würde die Information mitdrehen (-> 180° Drehung)

/* Initialisierung */

func Initialize()
{
  SetVisibility(VIS_Owner);
  return true;
}

public func Set(string szAction, object pOwner, int iColor)
{
	SetAction("Show", pOwner);
	if(iColor)
	  SetClrModulation(iColor, this);

	//Zusatzobjekt erstellen welches sich an dieses Objekt attached (im vordefinierten Abstand)
	if(szAction)
	{
		arrowIcon = CreateObject(TVIC, 0, 0, GetOwner());
		arrowIcon->SetAction(szAction, this);
		arrowIcon->SetVisibility(VIS_Owner);
	}

	return true;
}

public func GetIcon() { return arrowIcon; }

public func SetPos(int iAngle, int iRange)
{
	var xoff = -Sin(iAngle, iRange, 1000);
  var yoff = +Cos(iAngle, iRange, 1000);
  
  //Attached Pfeil durch Vertex positionieren
  SetVertex(0, 0, xoff);
  SetVertex(0, 1, yoff);
  
  SetR(iAngle/1000);
  
  //Icon relativ zu Pfeil positionieren
  if(arrowIcon)
  {
		arrowIcon->SetVertex(0, 0, -Sin(iAngle, iRange-25, 1000));
		arrowIcon->SetVertex(0, 1, +Cos(iAngle, iRange-25, 1000));
	}

  return true;
}

public func Destruction()
{
	//Icon löschen
	if(arrowIcon)
		RemoveObject(arrowIcon);
	
	return true;
}

static const TVAR_VIS_All = 0;      //Für alle sichtbar
static const TVAR_VIS_Players = 1;  //Nur für angegebene Spieler
static const TVAR_VIS_Enemys = 2;   //Nur für Gegner der angegebenen Spieler
static const TVAR_VIS_Allies = 3;   //Nur für verbündete der angegebenen Spieler
static const TVAR_VIS_Teams = 4;    //Nur für die angegebenen Teams
static const TVAR_VIS_Objects = 5;  //Nur für die Objectowner
static const TVAR_VIS_None = 6;			//Für keinen Sichtbar
static const TVAR_VIS_TEnemys = 7;  //Nur für Gegner eines Teams sichtbar
static const TVAR_VIS_TAllies = 8;  //Nur für Verbündete eines Teams sichtbar

//Verarbeitet Sichtbarkeits-Informationen
global func TVAR_ProcessVis(array aVisConf)
{
	if(!aVisConf)
		return;

	var iVisMode = aVisConf[0], aArray = [], aNew = [];
	for(var i = 1; i < GetLength(aVisConf); i++)
		aNew[GetLength(aNew)] = aVisConf[i];
	
	aVisConf = aNew;
	
	//Alle Spieler durchgehen und Spieler aussortieren
	for(var i = 0; i < GetPlayerCount(); i++)
	{
		var plr = GetPlayerByIndex(i);
		if(iVisMode == TVAR_VIS_Players) //Vordefinierte Spieler
		{
			if(GetIndexOf(plr, aVisConf) == -1)
				continue;
		}
		else if(iVisMode == TVAR_VIS_Enemys || iVisMode == TVAR_VIS_Allies) //Verfeindete oder Verbündete Spieler
		{
			var a = (iVisMode == TVAR_VIS_Allies); 
			for(var p in aVisConf)
				if((Hostile(plr, p) && a) || (Hostile(plr, p) && !a))
					break;
			
			if((Hostile(plr, p) && a) || (Hostile(plr, p) && !a))
				continue;
		}
		else if(iVisMode == TVAR_VIS_Teams) //Spezielle Teams
		{
			if(GetIndexOf(GetPlayerTeam(p), aVisConf) == -1)
				continue;
		}
		else if(iVisMode == TVAR_VIS_Objects) //Objektbesitzer
		{
			var c = false;
			for(var o in aVisConf)
				if(GetOwner(o) == plr)
				{
					c = true;
					break;
				}
			
			if(!c)
				continue;
		}
		else if(iVisMode == TVAR_VIS_None) //Keiner
			break;
		else if(iVisMode == TVAR_VIS_TEnemys || iVisMode == TVAR_VIS_TAllies) //Team-Verfeindungen oder Team-Verbündete
		{
			var c = false, a = (iVisMode == TVAR_VIS_TAllies);
			for(var t in aVisConf)
				if((Hostile(GetTeamMemberByIndex(t), plr) && a) || (Hostile(GetTeamMemberByIndex(t), plr) && !a))
				{
					c = true;
					break;
				}
			
			if(!c)
				continue;
		}
		
		aArray[GetLength(aArray)] = plr;
	} 
	
	return aArray;
}

global func TVAR_Not(array aVisConf)
{
	var aArray = TVAR_ProcessVis(aVisConf), iMode = aVisConf[0];
	aVisConf = [TVAR_VIS_Players];
	for(var i = 0; i < GetPlayerCount(); i++)
	{
		var plr = GetPlayerByIndex(i);
		if(GetIndexOf(plr, aArray) == -1)
			aVisConf[GetLength(aVisConf)] = plr;
	}
	
	return aVisConf;
}




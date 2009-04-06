/* StippelMatch-Append */
#strict
#appendto GSM_

/* Temporäres StippelMatch-Append, da nativer Support erst in 3.0 kommt. */
/* Baut das Spielziel speziell für PipelinePanic um.                     */


//Wie viele Stippel müssen da sein, bis das Gebiet eingenommen ist?
private func AreaMax(int i)
{
  if(i == 0) return(1);
  if(i == 8) return(1);
  if(i == 4) return(30);
  return(15);
}

//Name des Gebietes.
private func AreaName(int i)//muss eingestellt sein!
{
  if(i == 0) return("A");
  if(i == 1) return("B");
  if(i == 2) return("C");
  if(i == 3) return("D");
  if(i == 4) return("Z");
  if(i == 5) return("H");
  if(i == 6) return("G");
  if(i == 7) return("F");
  if(i == 8) return("E");
  return(0);
}

//Koordinaten jedes Gebiets.
private func AreaCoordinates(int i, int & x, int & y, int & w, int & h)
{
  if(i == 0)
  {
    x = 0;
    y = 110;
    w = 420;
    h = 230;
    return(true);
  }
  
  if(i == 1)
  {
    x = 320;
    y = 110;
    w = 500;
    h = 230;
    return(true);
  }
  
  if(i == 2)
  {
    x = 920;
    y = 90;
    w = 350;
    h = 250;
    return(true);
  }
  
  if(i == 3)
  {
    x = 1270;
    y = 110;
    w = 370;
    h = 230;
    return(true);
  }
  
  if(i == 4)
  {
    x = 1640;
    y = 90;
    w = 760;
    h = 250;
    return(true);
  }
  
  if(i == 5)
  {
    x = 2400;
    y = 110;
    w = 370;
    h = 230;
    return(true);
  }
  
  if(i == 6)
  {
    x = 2770;
    y = 90;
    w = 350;
    h = 250;
    return(true);
  }
  
  if(i == 7)
  {
    x = 3120;
    y = 110;
    w = 500;
    h = 230;
    return(true);
  }
  
  if(i == 8)
  {
    x = 3620;
    y = 110;
    w = 420;
    h = 230;
    return(true);
  }
  
  return(false);
}



private func StippelValue(type)//Fürs Stippelzählen... :)
{
  if(type == ST5B) return(1);
  if(type == SK5B) return(0);
  if(type == KG5B) return(10);
  if(type == MI5B) return(3);
  if(type == MB5B) return(0);
  if(type == MF5B) return(5);
  return(1);
}






///Internes

local aAreas;//A B C D   Z   H G F E
local go;

protected func Initialize()
{
  _inherited();
  
  this()->~StippelReset();
  aAreas = CreateArray();
  SetPosition();
  LocalN("sp_x") = -2;//Vordefinierte Verteilung.
  LocalN("configurated") = true;
  AddEffect("IntUpdate", this(), 1, 35, this());
}

protected func FxIntUpdateTimer(object target, int number)
{
  Update();
}

protected func FxIntBroadcastTimer(object target, int number)
{
  Broadcast();
}



/*private ShowAreas()
{
  var i,rgb;
  for(var a in aAreas)
  {
    rgb = InterpolateRGBa2(RGB(0,255,0),RGB(255,255,0),RGB(255,0,0),0,100,a);
    SetScoreboardData (i,0,Format("<c %x>%s</c>",rgb,AreaName(i)),i);
    i++;
  }
}*/

private func ShowAreas()
{
  return();
  var i,rgb,text = "@";
  for(var a in aAreas)
  {
    rgb = InterpolateRGBa2(RGB(0,255,0),RGB(255,255,0),RGB(255,0,0),0,100,a);
    text = Format("%s<c %x>%s</c> ",text,rgb,AreaName(i));
    i++;
  }
  
  //HelpMessageCMC(text,NO_OWNER,"ShowAreas");
  CustomMessage(text,0,NO_OWNER,0,100,0,0,0,true);
}

public func GetAreaState(int i){return(aAreas[i]);}

public func Broadcast()
{
  if(CheckA(aAreas,0))
  {
    GameCall("AreasOvertaken");
    return();
  }
  
  if(CheckA(aAreas,100))
  {
    GameCall("AreasCleared");
    return();
  }

  var i;
  while(AreaName(i))
  {
    GameCall(Format("Area%dEvent",i),aAreas[i]);
    i++;
  }
}

public func Update()
{
  var i;
  while(AreaName(i))
  {
    aAreas[i] = BoundBy(CheckArea(i)*100/AreaMax(i), 0, 100);//in Prozent :D
    i++;
  }
  
  ShowAreas();
}

public func IsFulfilled()
{
  if(!go)
  {
    if(ObjectCount2(Find_Func("IsStippel")))
    {
      AddEffect("IntBroadcast", this(), 1, 35, this());
      go = true;
    }
    else
      return(false);
  }
  
  Update();
  
  if(CheckA(aAreas,0))
  {
    Log(Format("<c %x>$Won$</c>",RGB(0,255)));
    Sound ("won",true,0,200);
    this()->~ShowAwards();
    Schedule ("GameOver()",300); 
    return(true);
  }
  
  if(CheckA(aAreas,100))
  {
    Log(Format("<c %x>$Loosed$</c>",RGB(255)));
    this()->~ShowAwards();
    Schedule ("GameOver()",300); 
    return(false);
  }
}

private func CheckA(array a, v)
{
  for(var b in a)
  {
    if(b != v)
      return(false);
  }
  return(true);
}

private func CheckArea(int i)
{
  var j,x,y,w,h;
  AreaCoordinates(i,x,y,w,h);
  
  for(var obj in FindObjects(Find_InRect(x,y,w,h),Find_Func("IsStippel")))
  {
    j += StippelValue(GetID(obj));
  }
  
  return(j);
}



  
protected func Activate(iPlayer)
{
  if(IsFulfilled())
      return(MessageWindow("$MsgGoalFulfilled$", iPlayer));
  
  var i,rgb,text = "";
  for(var a in aAreas)
  {
    rgb = InterpolateRGBa2(RGB(0,255,0),RGB(255,255,0),RGB(255,0,0),0,100,a);
    text = Format("%s|<c %x>%s</c>",text,rgb,AreaName(i));
    i++;
  }
  
  MessageWindow(
            Format("$MsgGoalUnfulfilled$|%s",
                  GetLength (FindObjects(Find_Func ("IsStippel"))),
                  text),
            iPlayer);
}
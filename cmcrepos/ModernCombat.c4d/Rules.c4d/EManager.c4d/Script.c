/*-- Effekt-Manager --*/

#strict

//Mit der globalen Funktion EffectLevel kann der Effektgrad abgefragt werden!
global func EffectLevel()
{
  var em = FindObject(EM4K);
  
  if(!em)
    em = CreateObject(EM4K,0,0,NO_OWNER);
  
  return(em->Get());
}

//HasEMCallback
//EMCallback(int iSetting)


static const EM4K_Low      = NO_OWNER;//-1 mag er nicht. -1 == 38 Ô.o Sinn?
static const EM4K_Normal   = 0;
static const EM4K_High     = 1;
static const EM4K_VeryHigh = 2;



local setting,auto,callbacks;

public func Initialize()
{
  if(FindObject(GetID()))
    ScheduleCall(this(),"RemoveObject",1); 
    
  callbacks = [];
    
  for(var i, idC; idC = GetDefinition(i) ; i++)
    if(DefinitionCall(idC, "HasEMCallback"))
      callbacks[GetLength(callbacks)] = idC;
    
  setting = EM4K_VeryHigh;
  SetPosition();
    
  return(true);
}

public func Set(int iSetting)
{
  setting = BoundBy(iSetting, EM4K_Low, EM4K_VeryHigh);
  
  for(var idC in callbacks)
    for(var obj in FindObjects(Find_ID(idC)))
      obj->~EMCallback(setting);
}

public func SetAuto(bool bAuto)
{
  auto = bAuto;
}

public func Get()
{
  return(setting);
}

public func GetAuto()
{
  return(auto);
}

protected func Activate(int iPlayer, int iSelection)
{
  if(iPlayer != 0)
  {
    MessageWindow("$MsgNoAdmin$", iPlayer);
    return(true);
  }
    
  var clonk = GetCursor(iPlayer);
  if(!clonk)//Hey, kann ja echt sein! :X
  {
    Sound("Error",true,0,0,iPlayer+1);
    return(true);
  }
  
  //Menü
  CloseMenu(clonk);
  CreateMenu (GetID(),clonk,this(),0,GetName(),0,C4MN_Style_Normal);

  if(setting <= EM4K_Low)
    AddMenuItem ("$Low$","Switch",GetID(),clonk,0,iPlayer,"$DescLow$",2,1);
    
  if(setting == EM4K_Normal)
    AddMenuItem ("$Normal$","Switch",GetID(),clonk,0,iPlayer,"$DescNormal$",2,2);
  
  if(setting == EM4K_High)
    AddMenuItem ("$High$","Switch",GetID(),clonk,0,iPlayer,"$DescHigh$",2,3);
  
  if(setting >= EM4K_VeryHigh)
    AddMenuItem ("$VeryHigh$","Switch",GetID(),clonk,0,iPlayer,"$DescVeryHigh$",2,4);
  
  if(auto)
    AddMenuItem ("$NoAuto$","SwitchAuto",GetID(),clonk,0,iPlayer,"$DescAuto$",2,6);
  else
    AddMenuItem ("$Auto$","SwitchAuto",GetID(),clonk,0,iPlayer,"$DescAuto$",2,5);
    
  SelectMenuItem (iSelection,clonk);
    
  return(true);
}

protected func Switch(idItem, iParameter, bRight)
{   
  var v = -1;
  if(bRight)
    v = +1;
    
  var i = setting + v;
  
  if(i < EM4K_Low)
    i = EM4K_VeryHigh;
  if(i > EM4K_VeryHigh)
    i = EM4K_Low;
  
  Set(i);
  
  Sound("Grab",true,0,0,iParameter);
  Activate(iParameter,0);
}

protected func SwitchAuto(idItem, iParameter, bRight)
{
  auto = !auto;
  
  Sound("Connect",true,0,0,iParameter);
  Activate(iParameter,1);
}
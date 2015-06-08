/*-- Neues Script --*/

#strict 2

#appendto 1HUD

// Sectionwechsel - zu speichernde Objekte.
public func SpecialSaveObjects()
{
  var chars = [];
  for(var char in CharsWAmmo)
  	chars[GetLength(chars)] = char;
  
  for(var char in CharsMaxAmmo)
  	chars[GetLength(chars)] = char;
  	
  for(var char in CharsClonkAmmo)
  	chars[GetLength(chars)] = char;
  	
  for(var char in CharsGrenade)
  	chars[GetLength(chars)] = char;
  	
  chars[GetLength(chars)] = CharEqS;
  chars[GetLength(chars)] = rechargebar;
  
  return chars;
}

public func Destruction()
{
  for(var char in CharsWAmmo)
  	if(char)
  		RemoveObject(char);
  
  for(var char in CharsMaxAmmo)
  	if(char)
  		RemoveObject(char);
  
  for(var char in CharsClonkAmmo)
  	if(char)
  		RemoveObject(char);
  
  for(var char in CharsGrenade)
  	if(char)
  		RemoveObject(char);
  
  if(CharEqS)
    RemoveObject(CharEqS);
  if(rechargebar)
    RemoveObject(rechargebar);
}

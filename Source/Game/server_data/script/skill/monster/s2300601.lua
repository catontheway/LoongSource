--ʹ�ü��ܺ���

function s2300601_Cast(mapid, instanceid, skillid, ownerid)
     local TypeID = cre.GetCreatureTypeID(mapid,instanceid, ownerid)
     if TypeID == 1005459 then
     local temp = math.random(1,100)
	    if temp>=1 and temp<=20 then
		    --20%���ʺ���
                 cre.MonsterSay(mapid, instanceid, ownerid, 20092)
           end
     end
     return 0
end


--ע��

aux.RegisterSkillEvent(2300601, 1, "s2300601_Cast")

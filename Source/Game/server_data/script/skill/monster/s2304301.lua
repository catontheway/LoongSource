--ʹ�ü��ܺ���

function s2304301_Cast(mapid, instanceid, skillid, ownerid)
     local TypeID = cre.GetCreatureTypeID(mapid,instanceid, ownerid)
     if TypeID == 1005420 then
     local temp = math.random(1,100)
	    if temp>=1 and temp<=20 then
		    --20%���ʺ���
                 cre.MonsterSay(mapid, instanceid, ownerid, 20065)
           end
     end
     return 0
end


--ע��

aux.RegisterSkillEvent(2304301, 1, "s2304301_Cast")

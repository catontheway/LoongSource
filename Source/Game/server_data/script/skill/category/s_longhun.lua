--���ܡ������걬�������꼤��
--���걬������ID����9001301
--���꼤������ID����9001401

--���걬�������Ƿ����ʹ��
function s9001301_CanCast(mapid, instanceid, skillid, ownerid, dummy)
    local num = role.GetLongHunID(ownerid)  --������������������ID
    if num == nil or num == 0 then 
        return 7
    else
        return 0
    end
end

--���걬������Ч��
function s9001301_Cast(mapid, instanceid, skillid, ownerid)
    role.CastLongHun(ownerid, skillid)
    return 1
end

--���꼤���Ƿ��ʹ��
function s9001401_CanCast(mapid, instanceid, skillid, ownerid, dummy)
	local bRet = role.ProlifeLongHun(ownerid, 90013, 0)
	if bRet == nil then
		return 7
	else
		return 0
	end
end

--���꼤��Ч��
function s9001401_Cast(mapid, instanceid, skillid, ownerid)
	role.ProlifeLongHun(ownerid, 90013, 1)
	return 1
end

--ע��
aux.RegisterSkillEvent(9001301, 0, "s9001301_CanCast")
aux.RegisterSkillEvent(9001301, 1, "s9001301_Cast")
aux.RegisterSkillEvent(9001401, 0, "s9001401_CanCast")
aux.RegisterSkillEvent(9001401, 1, "s9001401_Cast")
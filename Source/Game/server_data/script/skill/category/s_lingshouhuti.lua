
--ʹ�����޻��塢�߼����弼�� ���ݽ�ɫ�������ޣ���Ӳ�ͬ�ָ�BUFF
--1400701	����
--1400702	�߼�����

function s1400701_Cast(mapid, instanceid, skillid, ownerid, roleid)

local k = role.GetRoleAttValue(mapid, instanceid, roleid, 8)
	if k >= 100000 then
	unit.AddBuff(mapid, instanceid,roleid,8002601,roleid)
	else
	unit.AddBuff(mapid, instanceid,roleid,8001301,roleid)
	end

end
--ע��

aux.RegisterPetSkillEvent(1400701, 1, "s1400701_Cast")


function s1400702_Cast(mapid, instanceid, skillid, ownerid, roleid)

local k = role.GetRoleAttValue(mapid, instanceid, roleid, 8)
	if k >= 100000 then
	unit.AddBuff(mapid, instanceid,roleid,8002701,roleid)
	else
	unit.AddBuff(mapid, instanceid,roleid,8001401,roleid)
	end

end
--ע��
--[[
1401101
1401401
1401701
1402001
--]]
aux.RegisterPetSkillEvent(1400702, 1, "s1400702_Cast")
aux.RegisterPetSkillEvent(1401101, 1, "s1400702_Cast")
aux.RegisterPetSkillEvent(1401401, 1, "s1400702_Cast")
aux.RegisterPetSkillEvent(1401701, 1, "s1400702_Cast")
aux.RegisterPetSkillEvent(1402001, 1, "s1400702_Cast")
aux.RegisterPetSkillEvent(1402301, 1, "s1400702_Cast")
aux.RegisterPetSkillEvent(1402601, 1, "s1400702_Cast")
aux.RegisterPetSkillEvent(1402901, 1, "s1400702_Cast")














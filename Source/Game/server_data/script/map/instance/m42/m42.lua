--ScriptData��������1,2
--ScriptData ����3-9 ���ڼ�ħ̨

--����Data��������10,11(С�֣�BOSS)
--12������ȡ����������

--���븱����һ��һ��buff�����������˺�

function m42_OnPlayerEnter(MapID, InstanceID,RoleID)
	 unit.AddBuff(MapID, InstanceID,RoleID, 2017301,RoleID)

	--���븱��ʱɾ����ҵ�����buff
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	 end
end

--ע��
aux.RegisterMapEvent("m42", 2, "m42_OnPlayerEnter")



--����11 �����ͼ���� 2160��ÿ5���յ�һ�η����˺� BUFF2017401����Ѫ30%
function m42_OnEnterArea(MapID, InstanceID, RoleID, ObjID)
	if ObjID == 2160 then
		local lTime = map.GetMapScriptData(MapID,InstanceID,1,2) --����Data����2
		if lTime == nil then
			lTime = 0
		end
		local cTime = os.time() --��ǰʱ��
		local dTime = cTime - lTime --�ϴδ���������ʱ��
		if dTime > 5 then --�жϴ�������Ƿ����5����
			unit.AddBuff(MapID, InstanceID, RoleID, 2017401, RoleID)
			map.SetMapScriptData(MapID,InstanceID,1,2,cTime)
		end
	end
end

aux.RegisterMapEvent("m42", 7, "m42_OnEnterArea")





--����������ʯ ID��3040527 BUFF��� ID��2017302

function c3040527_OnInvest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)

	unit.AddBuff(MapID, InstanceID, RoleID, 2017302, RoleID)

end
aux.RegisterCreatureEvent(3040527, 6, "c3040527_OnInvest")




--���鶫����ʯ ID��3040528 BUFF��� ID��2017302

function c3040528_OnInvest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)

	unit.AddBuff(MapID, InstanceID, RoleID, 2017302, RoleID)

end
aux.RegisterCreatureEvent(3040528, 6, "c3040528_OnInvest")




--�����ϼ���ʯ ID��3040529  BUFF��� ID��2017302

function c3040529_OnInvest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)

	unit.AddBuff(MapID, InstanceID, RoleID, 2017302, RoleID)

end
aux.RegisterCreatureEvent(3040529, 6, "c3040529_OnInvest")



--���鱱����ʯ ID��3040530 BUFF��� ID��2017302

function c3040530_OnInvest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)

	unit.AddBuff(MapID, InstanceID, RoleID, 2017302, RoleID)

end
aux.RegisterCreatureEvent(3040530, 6, "c3040530_OnInvest")



--���븱���󣬲��ּ����޷�ʹ��
--m42_SkillLimit = {}
--m42_SkillLimit[11005] = true
--m42_SkillLimit[11009] = true
--m42_SkillLimit[11012] = true
--m42_SkillLimit[11014] = true
--m42_SkillLimit[90012] = true

--function m42_CanUseSkill(MapID, InstanceID, SkillTypeID)
--    if m42_SkillLimit[SkillTypeID] then
--        return false
--    end
--    return true
--end

--aux.RegisterMapEvent("m42", 21, "m42_CanUseSkill")



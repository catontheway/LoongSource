--SciptData ����0 ������901
--SciptData ����1-7 ���ڼ�ħ̨

m40_DataType = {}
m40_DataType["SumNum"] = 0
m40_DataType["roleid1"] = 1
m40_DataType["roleid2"] = 2
m40_DataType["roleid3"] = 3
m40_DataType["roleid4"] = 4
m40_DataType["roleid5"] = 5
m40_DataType["roleid6"] = 6

--8���ڻ�ɱboss�����¼
--9���ڴ�����ˢ��
--10���ڼ�¼������������齱��
--11���ڻ�ɱС������

--m40 SciptData ��¼������901 ����0��ֵΪ0��ˢ��
function m40_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)

--������� 901�� �����꣺x 726 y 5190 z 312 ˢ�� 1005466�� �������� c01 Ѳ��
	if ObjID == 901 then
		local Activated0 = map.GetMapScriptData(MapID,InstanceID,1,0) --����9��ֵΪ0��ˢ��
		if Activated0 == 0 then --�ж��Ƿ񱻴�����
		map.SetMapScriptData(MapID,InstanceID,1,9,1)
		map.MapCreateColCreature(MapID, InstanceID, 1005466, 726, 5190, 312, 1, "c01")
		end
	end
end
aux.RegisterMapEvent("m40",6,"m40_OnEnterTrigger")

--��ɫ���븱���󣬲��ּ����޷�ʹ��
--m40_SkillLimit = {}
--m40_SkillLimit[11005] = true
--m40_SkillLimit[11009] = true
--m40_SkillLimit[11012] = true
--m40_SkillLimit[11014] = true
--m40_SkillLimit[90012] = true

--function m40_CanUseSkill(MapID, InstanceID, SkillTypeID)
--    if m40_SkillLimit[SkillTypeID] then
--        return false
--    end
--    return true
--end

--aux.RegisterMapEvent("m40", 21, "m40_CanUseSkill")

--���븱��ʱɾ����ҵ�����buff
function m40_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	 end
end

--ע��
aux.RegisterMapEvent("m40", 2, "m40_OnPlayerEnter")

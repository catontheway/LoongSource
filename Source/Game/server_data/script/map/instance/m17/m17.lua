--���븱���󣬲��ּ����޷�ʹ��
--m17_SkillLimit = {}
--m17_SkillLimit[11005] = true
--m17_SkillLimit[11009] = true
--m17_SkillLimit[11012] = true
--m17_SkillLimit[11014] = true
--m17_SkillLimit[90012] = true

--function m17_CanUseSkill(MapID, InstanceID, SkillTypeID)
--    if m17_SkillLimit[SkillTypeID] then
--        return false
--    end
--    return true
--end

--aux.RegisterMapEvent("m17", 21, "m17_CanUseSkill")

--���븱��ʱɾ����ҵ�����buff
function m17_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	 end
end

--ע��
aux.RegisterMapEvent("m17", 2, "m17_OnPlayerEnter")

--RLC103 ��֤���ʱ�ĵ���Ϊ0
--�������ʱ
function q10103_OnComplete(MapID, InstanceID, QuestID, OwnerID, EnderID)
	local i=role.GetRoleAttValue(MapID, InstanceID, OwnerID, 74)
	if i > -45 then
	    role.ModRoleAttValue(MapID, InstanceID, OwnerID, 74, -45-i)
	end
end

--ע��
aux.RegisterQuestEvent(10103, 1, "q10103_OnComplete")
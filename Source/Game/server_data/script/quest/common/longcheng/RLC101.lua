--ʹ�û��Ͼ� 2610181����û��Ͼ��ֳ��� 2610184
function I2610181_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	
      role.QuestAddRoleItem(MapID, InstanceID, TargetID, 10101, 2610184, 1, -1, 102)

end


aux.RegisterItemEvent(2610181, 1, "I2610181_QuestUsable")


--�������ʱ ��֤���ʱ�ĵ���Ϊ0
function q10101_OnComplete(MapID, InstanceID, QuestID, OwnerID, EnderID)
	local i=role.GetRoleAttValue(MapID, InstanceID, OwnerID, 74)
	if i > -20 then
	    role.ModRoleAttValue(MapID, InstanceID, OwnerID, 74, -20-i)
	end
end

--ע��
aux.RegisterQuestEvent(10101, 1, "q10101_OnComplete")
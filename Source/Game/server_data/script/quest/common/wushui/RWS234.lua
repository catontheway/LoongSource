--ƿ����(һ)��13234
--����1��ʹ����Ʒ��ɫƿ[2613121]��ӵ�����ƿ����(һ)[13234]
--������������ 13234 ����ֱ�Ӹ������ 2613125*1���жϱ����Ƿ���1���ո�

--��Ʒ��ɫƿʹ��Ч������
function i2613121_QuestGiver(MapID, InstanceID, TypeID, TargetID)
	local IsCom = role.IsRoleCompleteQuest(MapID, InstanceID, TargetID, 13234)
		if IsCom == true then
			local bget = role.AddRoleItem(MapID, InstanceID, TargetID, 2613125, 1, -1, 8, 420)
			if bget == 0 then
			role.RemoveFromRole(MapID, InstanceID, TargetID, 2613121, 1, 1000)
			end
		else
			role.AddQuest(TargetID, 13234)
		end
end

--ע��
aux.RegisterItemEvent(2613121, 1, "i2613121_QuestGiver")

function I2613121_CanAdd(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false

	local IsCom = role.IsRoleCompleteQuest(MapID, InstanceID, TargetID, 13234)

	if IsCom == true then
	--�жϱ������пռ��Ƿ��㹻
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 1) then
			--��ʾ��ұ����ռ䲻��
			bRet = 40
		end
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(2613121, 0, "I2613121_CanAdd")

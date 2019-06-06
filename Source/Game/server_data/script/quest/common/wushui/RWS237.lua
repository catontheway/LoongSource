--ƿ����(��)��13237
--����1��ʹ����Ʒ��ɫƿ[2613124]��ӵ�����ƿ����(��)[13237]
--������������ 13237 ����ֱ�Ӹ������ 2613125*1���жϱ����Ƿ���1���ո�

--��Ʒ��ɫƿʹ��Ч������
function i2613124_QuestGiver(MapID, InstanceID, TypeID, TargetID)
	local IsCom = role.IsRoleCompleteQuest(MapID, InstanceID, TargetID, 13237)
		if IsCom == true then
			local bget = role.AddRoleItem(MapID, InstanceID, TargetID, 2613125, 1, -1, 8, 420)
			if bget == 0 then
			role.RemoveFromRole(MapID, InstanceID, TargetID, 2613123, 1, 1000)
			end
		else
			role.AddQuest(TargetID, 13237)
		end
end

--ע��
aux.RegisterItemEvent(2613124, 1, "i2613124_QuestGiver")

function I2613124_CanAdd(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false

	local IsCom = role.IsRoleCompleteQuest(MapID, InstanceID, TargetID, 13237)

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

aux.RegisterItemEvent(2613124, 0, "I2613124_CanAdd")

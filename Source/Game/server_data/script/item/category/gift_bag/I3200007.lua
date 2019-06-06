function I3200007_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, TargetID)

	rolelevel = math.floor(rolelevel/30)+1


	role.AddRoleItem(MapID, InstanceID, TargetID, 1140001, rolelevel, -1, 8, 420)

end

function I3200007_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3200007, 1, "I3200007_GiftBag")
aux.RegisterItemEvent(3200007, 0, "I3200007_CanUseGiftBag")

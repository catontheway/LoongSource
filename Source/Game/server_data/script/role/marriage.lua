-- ����ϵͳ��Ҫ��һЩȫ�ֶ���

--�����Ѷ�����
marriage_FriendShipVal  = 720  

--�����߽���Ǯ ��
marriage_GetMarriageDecSilver = 660000
--����������Ǯ ��
marriage_BreakMarriageDecSilver = 1000000
--��������Ǯ ��
marriage_WeddingDecSilver = 1000000

--1361107	����֮����
--1361108	����֮����
--1361109	δ����Ļ��
--1361110	δ��������
--1361111	������      �е�
--1361112	������      Ů��
--8990101	[��]����
--8990102	[��]����
--8990103	[��]����
--8990111	[��]��Ů
--8990112	[��]��Ů
--8990113	[��]��Ů

--���
function Marriage_GetMarriageEvent(MapID, InstanceID, ApplicantRoleID, TargetRoleID)

	--ɾ����ǰδ�������Ʒ
	role.RemoveFromRole(MapID, InstanceID, ApplicantRoleID, 1361109 , 1, 101)
	role.RemoveFromRole(MapID, InstanceID, ApplicantRoleID, 1361110 , 1, 101)
	
	role.RemoveFromRole(MapID, InstanceID, TargetRoleID, 1361109 , 1, 101)
	role.RemoveFromRole(MapID, InstanceID, TargetRoleID, 1361110 , 1, 101)
	
	--��ӻ����Ʒ 0:girl��1:boy(�����Ժ���չ)
	--�з� 	��ָ������	��Ʒ�������������װ
	--Ů��	��ָ����Ů	��Ʒ����Ů�Ի�ɴ��װ
	local MaleRoleID = 0 --�з���roleid
	local FeMaleRoleID = 0 --Ů����roleid

	if role.GetRoleSex(ApplicantRoleID) == 0 then
		--Ů
		role.AddRoleItem(MapID, InstanceID, ApplicantRoleID, 1361112	, 1, -1, 3, 104)
		role.AddRoleItem(MapID, InstanceID, ApplicantRoleID, 8990111	, 1, -1, 3, 104)

		--��
		role.AddRoleItem(MapID, InstanceID, TargetRoleID, 1361111	, 1, -1, 3, 104)
		role.AddRoleItem(MapID, InstanceID, TargetRoleID, 8990101	, 1, -1, 3, 104)

		MaleRoleID = TargetRoleID
		FeMaleRoleID = ApplicantRoleID
	else
		--��
		role.AddRoleItem(MapID, InstanceID, ApplicantRoleID, 1361111	, 1, -1, 3, 104)
		role.AddRoleItem(MapID, InstanceID, ApplicantRoleID, 8990101	, 1, -1, 3, 104)
		--Ů
		role.AddRoleItem(MapID, InstanceID, TargetRoleID, 1361112	, 1, -1, 3, 104)
		role.AddRoleItem(MapID, InstanceID, TargetRoleID, 8990111	, 1, -1, 3, 104)

		MaleRoleID = ApplicantRoleID
		FeMaleRoleID = TargetRoleID
	end

	--�������ߵ�ǮǮ
	role.DecRoleSilver(MapID, InstanceID, ApplicantRoleID, marriage_GetMarriageDecSilver, 101)
	
	--ȫ���㲥
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 500)	-- ȫ���㲥###���з�����###��Ů������ʽ��Ϊ���ޣ�ף���ǰ�ͷЯ�ϣ�����ͬ�ģ�
	msg.AddMsgEvent(MsgID, 2, MaleRoleID)	-- �з�roleid
	msg.AddMsgEvent(MsgID, 2, FeMaleRoleID)	-- Ů��roleid
	msg.DispatchWorldMsgEvent(MsgID)

end

--�����
function Marriage_GetMarriageCheckEvent(MapID, InstanceID, ApplicantRoleID, TargetRoleID)

	--�Ա��Ƿ���ͬ
	if role.GetRoleSex(ApplicantRoleID) == role.GetRoleSex(TargetRoleID) then
		return 2 --�Ա���ͬ
	end
	
	--�����Ѷ�
	if role.GetFriendShipValue(ApplicantRoleID,TargetRoleID) < marriage_FriendShipVal then
		return 3 --���ѶȲ���
	end
	
	--����������Ƿ��ѻ�
	if role.IsRoleMarried(ApplicantRoleID) == true then 
		return 4 --�������ѻ�
	end

	if role.IsRoleMarried(TargetRoleID) == true then 
		return 5 --���������ѻ�
	end

	--�����߽�Ǯ�Ƿ��㹻
	if role.GetRoleSilver(MapID,InstanceID,ApplicantRoleID) < marriage_GetMarriageDecSilver then
		return 7 --�����߱���ӵ��66��
	end

	--�Ƿ��л�ǰδ�������Ʒ
	local num1 = role.GetRoleItemNum(ApplicantRoleID,1361109)
	local num2 = role.GetRoleItemNum(ApplicantRoleID,1361110)

	local num3 = role.GetRoleItemNum(TargetRoleID,1361109)
	local num4 = role.GetRoleItemNum(TargetRoleID,1361110)

	if num1<1 or num2<1 or num3<1 or num4 <1 then
		return  10 --ȱ�ٱ�Ҫ��Ʒ
	end

	local num5 = role.GetBagFreeSize(ApplicantRoleID)
	local num6 = role.GetBagFreeSize(TargetRoleID)

	if num5<2 or num6<2 then
		return 12 --�����ռ䲻��
	end

	return 0 --�ɹ�

end

--ɾ������ָ
function Marriage_DelRing(MapID, InstanceID, ApplicantRoleID, TargetRoleID)
	--ɾ�������Ʒ 0:girl��1:boy(�����Ժ���չ)
	--�з� 	��ָ������
	--Ů��	��ָ����Ů
	--��ָ�п����Ѿ������� ����Ҫ�����п�ʽ�Ľ�ָ����ɾһ��
	if role.GetRoleSex(ApplicantRoleID) == 0 then
		--Ů����
		role.RemoveFromRole(MapID, InstanceID, ApplicantRoleID, 8990111	, 1, -1, 3, 104)
		role.RemoveFromRole(MapID, InstanceID, ApplicantRoleID, 8990112	, 1, -1, 3, 104)
		role.RemoveFromRole(MapID, InstanceID, ApplicantRoleID, 8990113	, 1, -1, 3, 104)
		--Ů�ֿ�
		role.RemoveFromWare(MapID, InstanceID, ApplicantRoleID, 8990111	, 1, -1, 3, 104)
		role.RemoveFromWare(MapID, InstanceID, ApplicantRoleID, 8990112	, 1, -1, 3, 104)
		role.RemoveFromWare(MapID, InstanceID, ApplicantRoleID, 8990113	, 1, -1, 3, 104)
		--Ůװ����
		role.RemoveFromEquipBar(MapID, InstanceID, ApplicantRoleID, 8990111	, 1, -1, 3, 104)
		role.RemoveFromEquipBar(MapID, InstanceID, ApplicantRoleID, 8990112	, 1, -1, 3, 104)
		role.RemoveFromEquipBar(MapID, InstanceID, ApplicantRoleID, 8990113	, 1, -1, 3, 104)
		--�б���
		role.RemoveFromRole(MapID, InstanceID, TargetRoleID, 	8990101	, 1, -1, 3, 104)
		role.RemoveFromRole(MapID, InstanceID, TargetRoleID, 	8990102	, 1, -1, 3, 104)
		role.RemoveFromRole(MapID, InstanceID, TargetRoleID, 	8990103	, 1, -1, 3, 104)
		--�вֿ�
		role.RemoveFromWare(MapID, InstanceID, TargetRoleID, 	8990101	, 1, -1, 3, 104)
		role.RemoveFromWare(MapID, InstanceID, TargetRoleID, 	8990102	, 1, -1, 3, 104)
		role.RemoveFromWare(MapID, InstanceID, TargetRoleID, 	8990103	, 1, -1, 3, 104)
		--��װ����
		role.RemoveFromEquipBar(MapID, InstanceID, TargetRoleID, 	8990101	, 1, -1, 3, 104)
		role.RemoveFromEquipBar(MapID, InstanceID, TargetRoleID, 	8990102	, 1, -1, 3, 104)
		role.RemoveFromEquipBar(MapID, InstanceID, TargetRoleID, 	8990103	, 1, -1, 3, 104)
	else
		--�б���
		role.RemoveFromRole(MapID, InstanceID, ApplicantRoleID, 8990101	, 1, -1, 3, 104)
		role.RemoveFromRole(MapID, InstanceID, ApplicantRoleID, 8990102	, 1, -1, 3, 104)
		role.RemoveFromRole(MapID, InstanceID, ApplicantRoleID, 8990103	, 1, -1, 3, 104)
		--�вֿ�
		role.RemoveFromWare(MapID, InstanceID, ApplicantRoleID, 8990101	, 1, -1, 3, 104)
		role.RemoveFromWare(MapID, InstanceID, ApplicantRoleID, 8990102	, 1, -1, 3, 104)
		role.RemoveFromWare(MapID, InstanceID, ApplicantRoleID, 8990103	, 1, -1, 3, 104)
		--��װ����
		role.RemoveFromEquipBar(MapID, InstanceID, ApplicantRoleID, 8990101	, 1, -1, 3, 104)
		role.RemoveFromEquipBar(MapID, InstanceID, ApplicantRoleID, 8990102	, 1, -1, 3, 104)
		role.RemoveFromEquipBar(MapID, InstanceID, ApplicantRoleID, 8990103	, 1, -1, 3, 104)
		--Ů����
		role.RemoveFromRole(MapID, InstanceID, TargetRoleID, 	8990111	, 1, -1, 3, 104)
		role.RemoveFromRole(MapID, InstanceID, TargetRoleID, 	8990112	, 1, -1, 3, 104)
		role.RemoveFromRole(MapID, InstanceID, TargetRoleID, 	8990113	, 1, -1, 3, 104)
		--Ů�ֿ�
		role.RemoveFromWare(MapID, InstanceID, TargetRoleID, 	8990111	, 1, -1, 3, 104)
		role.RemoveFromWare(MapID, InstanceID, TargetRoleID, 	8990112	, 1, -1, 3, 104)
		role.RemoveFromWare(MapID, InstanceID, TargetRoleID, 	8990113	, 1, -1, 3, 104)
		--Ůװ����
		role.RemoveFromEquipBar(MapID, InstanceID, TargetRoleID, 	8990111	, 1, -1, 3, 104)
		role.RemoveFromEquipBar(MapID, InstanceID, TargetRoleID, 	8990112	, 1, -1, 3, 104)
		role.RemoveFromEquipBar(MapID, InstanceID, TargetRoleID, 	8990113	, 1, -1, 3, 104)
	end
end

--���ͨ�ü��
function Marriage_BreakMarriageCommonCheck(ApplicantRoleID, TargetRoleID)

	--�Ա��Ƿ���ͬ
	if role.GetRoleSex(ApplicantRoleID) == role.GetRoleSex(TargetRoleID) then
		return 2 --�Ա���ͬ
	end
	
	--����������Ƿ��ѻ�
	if role.IsRoleMarried(ApplicantRoleID) == false then 
		return 11 --������δ��
	end

	return 0 --�ɹ�

end

--���
function Marriage_BreakMarriageEvent(MapID, InstanceID, ApplicantRoleID, TargetRoleID)

	--ɾ������ָ
	Marriage_DelRing(MapID, InstanceID, ApplicantRoleID, TargetRoleID)

	--�������ߵ�ǮǮ
	role.DecRoleSilver(MapID, InstanceID, ApplicantRoleID, marriage_BreakMarriageDecSilver, 101)

end

--�����
function Marriage_BreakMarriageCheckEvent(MapID, InstanceID, ApplicantRoleID, TargetRoleID)

	--���ͨ�ü��
	CommonCheckResult = Marriage_BreakMarriageCommonCheck(ApplicantRoleID, TargetRoleID)
	if CommonCheckResult ~= 0 then
		return CommonCheckResult
	end

	--�����߽�Ǯ�Ƿ��㹻
	if role.GetRoleSilver(MapID,InstanceID,ApplicantRoleID) < marriage_BreakMarriageDecSilver then
		return 7 --�����߱���ӵ���ܹ���������
	end

	return 0 --�ɹ�
end

--ǿ�����
function Marriage_ForceBreakMarriageEvent(MapID, InstanceID, ApplicantRoleID, TargetRoleID)

	--ɾ������ָ
	Marriage_DelRing(MapID, InstanceID, ApplicantRoleID, TargetRoleID)

	--�������߱����ڿ۳����鵤,typeid=1360118
	role.RemoveFromRole(MapID, InstanceID, ApplicantRoleID, 1360118, 1, 101);

end

--ǿ�������
function Marriage_ForceBreakMarriageCheckEvent(MapID, InstanceID, ApplicantRoleID, TargetRoleID)

	--���ͨ�ü��
	CommonCheckResult = Marriage_BreakMarriageCommonCheck(ApplicantRoleID, TargetRoleID)
	if CommonCheckResult ~= 0 then
		return CommonCheckResult
	end

	--����ɫ�����Ƿ��о��鵤��typeid=1360118
	if role.GetRoleItemNum(ApplicantRoleID, 1360118) <= 0 then
		return 13
	end

	return 0 --�ɹ�
end

aux.RegisterRoleEvent(11, "Marriage_GetMarriageEvent")
aux.RegisterRoleEvent(12, "Marriage_GetMarriageCheckEvent")

aux.RegisterRoleEvent(13, "Marriage_BreakMarriageEvent")
aux.RegisterRoleEvent(14, "Marriage_BreakMarriageCheckEvent")

aux.RegisterRoleEvent(42, "Marriage_ForceBreakMarriageEvent")
aux.RegisterRoleEvent(43, "Marriage_ForceBreakMarriageCheckEvent")

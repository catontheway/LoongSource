--1350018	�ɱ�
--1350001	[��]��ɱͼ
--1350002	[��]�ƾ�ͼ
--2000001	[��]��ͨʯ��
--2000011	[��]��ͨ����
--2000206	[��]��ͨƤ��
--2000211	[��]��ͨ�޲�
--2000006	[��]��ͨ��ʯ
--2000016	[��]��ͨ����
--2000201	[��]��ͨ�鲼
--2000216	[��]��ͨ��ɴ
--1350016	[��]���ǻ�����
--1350017	[��]��ޱ������
--1350019	�ǳ���
--1350051	[��]��������
--1350052	[��]�������
--1350053	[��]��а����
--1350054	[��]������צ
--2000002	[��]����ʯ��
--2000007	[��]������ʯ
--2000012	[��]��������
--2000017	[��]��������
--2000207	[��]����Ƥ��
--2000212	[��]�����޲�
--2000202	[��]�����鲼
--2000217	[��]������ɴ

--��������


function I1350031_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
    local  k = math.random(1,100)
		if  k >=1 and k <= 30  then
			--��###�����������####���������ƣ���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 418)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350018)
			msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350018, 5, -1, 8, 420)
		elseif k>=31 and k<=38 then
			--��###�����������####���������ƣ���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 418)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350018)
			msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350018, 10, -1, 8, 420)
		elseif k>=39 and k<=42 then
			--��###�����������####���������ƣ���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 418)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350018)
			msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350018, 20, -1, 8, 420)
		elseif k>=43 and k<=50 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350001, 3, -1, 8, 420)
		elseif k>=51 and k<=52 then
			--��###�����������####���������ƣ���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 418)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350002)
			msg.DispatchWorldMsgEvent(MsgID)
			role.AddRoleItem(MapID, InstanceID, TargetID, 1350002, 1, -1, 8, 420)
		elseif k>=53 and k<=57 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2000001, 5, -1, 8, 420)
		elseif k>=58 and k<=62 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2000011, 5, -1, 8, 420)
		elseif k>=63 and k<=67 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2000206, 5, -1, 8, 420)
		elseif k>=68 and k<=72 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2000211, 5, -1, 8, 420)
		elseif k>=73 and k<=77 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2000006, 5, -1, 8, 420)
		elseif k>=78 and k<=82 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2000016, 5, -1, 8, 420)
		elseif k>=83 and k<=87 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2000201, 5, -1, 8, 420)
		elseif k>=88 and k<=92 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2000216, 5, -1, 8, 420)
		elseif k>=93 and k<=97 then
			--��###�����������####���������ƣ���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 418)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350016)
			msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350016, 1, -1, 8, 420)
		elseif k>=98 and k<=99 then
			--��###�����������####���������ƣ���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 418)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350017)
			msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350017, 1, -1, 8, 420)
		else
			--��###�����������####���������ƣ���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 418)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350019)
			msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350019, 1, -1, 8, 420)
		end

end

function I1350031_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1350031, 1, "I1350031_GiftBag")
aux.RegisterItemEvent(1350031, 0, "I1350031_CanUseGiftBag")

--�������

function I1350032_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
    local  k = math.random(1,100)
		if  k >=1 and k <= 10  then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 419)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350018)
			msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350018, 5, -1, 8, 420)
		elseif k>=11 and k<=18 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 419)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350018)
			msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350018, 10, -1, 8, 420)
		elseif k>=19 and k<=20 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 419)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350018)
			msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350018, 20, -1, 8, 420)
		elseif k>=21 and k<=25 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2000002, 3, -1, 8, 420)
		elseif k>=26 and k<=30 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2000012, 3, -1, 8, 420)
		elseif k>=31 and k<=35 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2000207, 3, -1, 8, 420)
		elseif k>=36 and k<=40 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2000212, 3, -1, 8, 420)
		elseif k>=41 and k<=45 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2000007, 3, -1, 8, 420)
		elseif k>=46 and k<=50 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2000017, 3, -1, 8, 420)
		elseif k>=51 and k<=55 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2000202, 3, -1, 8, 420)
		elseif k>=56 and k<=60 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2000217, 3, -1, 8, 420)
		elseif k>=61 and k<=75 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 419)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350016)
			msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350016, 3, -1, 8, 420)
		elseif k>=76 and k<=95 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 419)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350017)
			msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350017, 1, -1, 8, 420)
		elseif k==96 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 419)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350019)
			msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350019, 1, -1, 8, 420)
		elseif k==97 then
			--��###���ڱ�����������������ı�֮####���������ƣ���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 419)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350051)
			msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350051, 1, -1, 8, 420)
		elseif k==98 then
			--��###���ڱ�����������������ı�֮####���������ƣ���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 419)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350052)
			msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350052, 1, -1, 8, 420)
		elseif k==99 then
			--��###���ڱ�����������������ı�֮####���������ƣ���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 419)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350053)
			msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350053, 1, -1, 8, 420)
		else
			--��###���ڱ�����������������ı�֮####���������ƣ���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 419)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.AddMsgEvent(MsgID, 4, 1350054)
			msg.DispatchWorldMsgEvent(MsgID)
		role.AddRoleItem(MapID, InstanceID, TargetID, 1350054, 1, -1, 8, 420)
		end
end

function I1350032_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1350032, 1, "I1350032_GiftBag")
aux.RegisterItemEvent(1350032, 0, "I1350032_CanUseGiftBag")

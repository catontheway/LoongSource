

--RoleDataType["guoqingdalibao_geren"]   = 153   --��¼�������ҿ���������������

Guoqingdalibao_jiedian = 0 --��¼�������й�������������
Guoqingdalibao_weishenqi = 0 -- ��¼��������α������������

function Guoqinglibao_PT(MapID, InstanceID, RoleID) -- ��ͨ���ʻ�õ���

	local  Level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local k = math.random(1000)
	if  k <= 130 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4022)   --������ʾ��һ�õ���xx
		msg.AddMsgEvent(MsgID, 4, 6000110)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		role.AddRoleItem(MapID, InstanceID, RoleID, 6000110, 1, -1, 8, 420) -- ��������
	elseif
		k >=131 and k <= 346 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4022)   --������ʾ��һ�õ���xx
		msg.AddMsgEvent(MsgID, 4, 6000111)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		role.AddRoleItem(MapID, InstanceID, RoleID, 6000111, 1, -1, 8, 420) -- ������
	elseif
		k >=347 and k <= 562 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4022)   --������ʾ��һ�õ���xx
		msg.AddMsgEvent(MsgID, 4, 6000112)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		role.AddRoleItem(MapID, InstanceID, RoleID, 6000112, 1, -1, 8, 420) -- ������
	elseif
		k >= 563 and k<= 874 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4023)   --������ʾ��һ�þ���xxx
		msg.AddMsgEvent(MsgID, 12, Level*50)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		role.AddRoleExp(MapID, InstanceID, RoleID, Level*50)
		role.AddRoleSilver(MapID, InstanceID, RoleID, Level*50, 102)
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4024)   --������ʾ���ʲôҲû�л��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end


function I6000113_OnUse(MapID, InstanceID, TypeID, RoleID)
	local	kaiqilibao= role.GetRoleScriptData(RoleID, 1, RoleDataType["guoqingdalibao_geren"]) --��¼���ÿ�տ����������������
	local  Level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local  Num = kaiqilibao +1
			role.SetRoleScriptData(RoleID, 1, RoleDataType["guoqingdalibao_geren"], Num)
			Guoqingdalibao_jiedian = Guoqingdalibao_jiedian + 1

	if Guoqingdalibao_jiedian == 250 then
		if Guoqingdalibao_weishenqi <=5 then -- ���α������������5�ѣ�������ͨ���ʻ�õ���
			local a = math.random(6000100,6000107)
			local MsgID = msg.BeginMsgEvent()   -- ���繫��
			msg.AddMsgEvent(MsgID, 100, 4025)--���xx�Ǳ������յ�xxλ�򿪹�����������ң���ϲ�������xx
			msg.AddMsgEvent(MsgID, 2, RoleID)
			msg.AddMsgEvent(MsgID, 12, Guoqingdalibao_jiedian)
			msg.AddMsgEvent(MsgID, 4, a) --��õ�������
			msg.DispatchWorldMsgEvent(MsgID)
			role.AddRoleItem(MapID, InstanceID, RoleID, a, 1, 4, 8, 420)
			Guoqingdalibao_weishenqi = Guoqingdalibao_weishenqi + 1
		else
			Guoqinglibao_PT(MapID, InstanceID, RoleID)
		end
	elseif
		Guoqingdalibao_jiedian%1000 == 0 then
		if Guoqingdalibao_weishenqi <=5 then -- ���α������������5�ѣ�������ͨ���ʻ�õ���
			local b = math.random(6000100,6000107)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 4025)--���xx�Ǳ������յ�xxλ�򿪹�����������ң���ϲ�������xx
			msg.AddMsgEvent(MsgID, 2, RoleID)
			msg.AddMsgEvent(MsgID, 12, Guoqingdalibao_jiedian)
			msg.AddMsgEvent(MsgID, 4, b) --��õ�������
			msg.DispatchWorldMsgEvent(MsgID)
			role.AddRoleItem(MapID, InstanceID, RoleID, b , 1, 4, 8, 420)
			Guoqingdalibao_weishenqi = Guoqingdalibao_weishenqi + 1
		else
			Guoqinglibao_PT(MapID, InstanceID, RoleID)
		end
	else
		if  Num%200 == 0 then
			if Guoqingdalibao_weishenqi <=5 then -- ���α������������5�ѣ�������ͨ���ʻ�õ���
				local c = math.random(1000)
				if c <= 640 then
					local ca = math.random(6000100,6000107)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 100, 4026)--���xxĿǰ�Ѿ�������xx��������������ϲ�������xx
					msg.AddMsgEvent(MsgID, 2, RoleID)
					msg.AddMsgEvent(MsgID, 12, Num)
					msg.AddMsgEvent(MsgID, 4, ca) --��õ�������
					msg.DispatchWorldMsgEvent(MsgID)
					role.AddRoleItem(MapID, InstanceID, RoleID, ca, 1, 4, 8, 420)
					Guoqingdalibao_weishenqi = Guoqingdalibao_weishenqi + 1
				else
					local cb = math.random(3400001,3400004)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 100, 4026)--���xxĿǰ�Ѿ�������xx��������������ϲ�������xx
					msg.AddMsgEvent(MsgID, 2, RoleID)
					msg.AddMsgEvent(MsgID, 12, Num)
					msg.AddMsgEvent(MsgID, 4, cb) --��õ�������
					msg.DispatchWorldMsgEvent(MsgID)
					role.AddRoleItem(MapID, InstanceID, RoleID, cb, 1, 4, 8, 420)
				end
			end
		elseif Num%20 == 0 then
			local d = math.random(1000)
			if d <= 330 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 4022)   --������ʾ��һ�õ���
				msg.AddMsgEvent(MsgID, 4, 3300103)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				role.AddRoleItem(MapID, InstanceID, RoleID, 3300103, 1, -1, 8, 420) -- �ɽ������
			elseif
				d >= 331 and d <= 661 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 4022)   --������ʾ��һ�õ���
				msg.AddMsgEvent(MsgID, 4, 3300102)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				role.AddRoleItem(MapID, InstanceID, RoleID, 3300102, 1, -1, 8, 420) -- �ɽ�ɽ���
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 4023)   --������ʾ��һ�þ���
				msg.AddMsgEvent(MsgID, 12, Level*50)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				role.AddRoleExp(MapID, InstanceID, RoleID, Level*50)
			end
		else
			Guoqinglibao_PT(MapID, InstanceID, RoleID)
		end
	end
end



function I6000113_CanUse(MapID, InstanceID, TypeID, RoleID)

	local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	local t = map.IsInArea(3017299663, -1, RoleID, 10994)	--����true��false
	local FreeSize = role.GetBagFreeSize(RoleID)
	if Guoqingjie == 1 then
		if Npcyuebing ~= 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 4013) -- �����ı���δ��ʼ�����������ı���ʼ��վ���ı����鸽��ʹ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 32, false
		elseif t ~= true  then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 4014) -- ����վ�������ı����鸽���ſ���ʹ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 32, false
		elseif (FreeSize < 1) then
			return 40, false
		else
			return 0, false
		end
	else 							--������ڼ������ڹ̶�ʱ��͵ص㿪���������֮�������ơ�
		if (FreeSize < 1) then
			return 40, false
		else
			return 0, false
		end
	end
end


aux.RegisterItemEvent(6000113, 1, "I6000113_OnUse")
aux.RegisterItemEvent(6000113, 0, "I6000113_CanUse")



















































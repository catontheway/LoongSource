

--����ڻ����

--3610220	�϶�
--3610221	����
--3610222	���


Zhongqiu_Creature = {}
Zhongqiu_Creature[1] = { TypeID=3610220,x=1000,y=20140,z=969 ,CreID=0}
Zhongqiu_Creature[2] = { TypeID=3610221,x=976,y=20140,z=969 ,CreID=0}
Zhongqiu_Creature[3] = { TypeID=3610222,x=955,y=20140,z=970 ,CreID=0}

yueguangbaohe = 0		-- �����¹ⱦ�� ȫ�ֱ���


function Zhongqiu_OnTimerMin(actID)

	if 	Zhongqiujie == 1 then

		local curmon = tonumber(os.date("%m"))
		local curday = tonumber(os.date("%d"))
		local curhour = tonumber(os.date("%H")) -- ��ȡ��������ǰСʱ��
		local curmin = tonumber(os.date("%M"))  -- ��ȡ��������ǰ������


		if (curmon == 9 and curday == 22) and (curhour == 19 and curmin == 55) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 3902)	--�ٹ�5���ӣ��¹ⱦ�оͿ��Կ����ˣ����λ�������׼��
		msg.DispatchWorldMsgEvent(MsgID)
		end

		if (curmon == 9 and curday > 22) and (curhour == 18 and curmin == 55) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 3902)	--�ٹ�5���ӣ��¹ⱦ�оͿ��Կ����ˣ����λ�������׼��
		msg.DispatchWorldMsgEvent(MsgID)
		end
		if (curmon == 9 and curday >= 22) and (curhour == 20 and curmin == 55) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 3903)	--�¹ⱦ�еĿ���ʱ�仹ʣ��5���ӣ����λ���ץ��ʱ��
		msg.DispatchWorldMsgEvent(MsgID)
		end
		for i = 1 , 3 do
			if Zhongqiu_Creature[i].CreID == 0 then
			Zhongqiu_Creature[1].CreID =  map.MapCreateColCreature(3017299919, -1, Zhongqiu_Creature[1].TypeID, Zhongqiu_Creature[1].x, Zhongqiu_Creature[1].y, Zhongqiu_Creature[1].z, 1)
			Zhongqiu_Creature[2].CreID =  map.MapCreateColCreature(3017299919, -1, Zhongqiu_Creature[2].TypeID, Zhongqiu_Creature[2].x, Zhongqiu_Creature[2].y, Zhongqiu_Creature[2].z, 1)
			Zhongqiu_Creature[3].CreID =  map.MapCreateColCreature(3017299919, -1, Zhongqiu_Creature[3].TypeID, Zhongqiu_Creature[3].x, Zhongqiu_Creature[3].y, Zhongqiu_Creature[3].z, 1)
			end
		end

	end

end

function Zhongqiu_OnStart(actID)
	if Zhongqiujie == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 3904)	--����ڻ�Ѿ���ʼ��,���λ��ҵ���ˮ�ҵ����϶�[1000,969]�����[955��970]������[976��969]�����ǹ��ȼѽڣ�
		msg.DispatchBroadcast(MsgID, -1, -1, -1)
		Zhongqiu_Creature[1].CreID =  map.MapCreateColCreature(3017299919, -1, Zhongqiu_Creature[1].TypeID, Zhongqiu_Creature[1].x, Zhongqiu_Creature[1].y, Zhongqiu_Creature[1].z, 1)
		Zhongqiu_Creature[2].CreID =  map.MapCreateColCreature(3017299919, -1, Zhongqiu_Creature[2].TypeID, Zhongqiu_Creature[2].x, Zhongqiu_Creature[2].y, Zhongqiu_Creature[2].z, 1)
		Zhongqiu_Creature[3].CreID =  map.MapCreateColCreature(3017299919, -1, Zhongqiu_Creature[3].TypeID, Zhongqiu_Creature[3].x, Zhongqiu_Creature[3].y, Zhongqiu_Creature[3].z, 1)
	end
end

function Zhongqiu_OnEnd(actID)
	if Zhongqiujie == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 3905)	--����ڻ�Ѿ�������,��л��ҵĲ��롣
		msg.DispatchWorldMsgEvent(MsgID)
		for i = 1 , 3 do
			if Zhongqiu_Creature[i].CreID ~= 0 then

				map.MapDeleteCreature(3017299919, -1, Zhongqiu_Creature[i].CreID)
				Zhongqiu_Creature[i].CreID =0
			end
		end
		Zhongqiujie = 0
	end
end

--ע��û������¼�
aux.RegisterActEvent(152, 4, "Zhongqiu_OnTimerMin")	--�ID
aux.RegisterActEvent(152, 2, "Zhongqiu_OnStart")	--�ID
aux.RegisterActEvent(152, 3, "Zhongqiu_OnEnd")		--�ID




-- ʹ������ԭʯ

function Yueliangshi_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
		return bRet, bIgnore
	elseif yueguangbaohe == 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 3906)  -- ��ʾ�������ԭʯֻ���ڻ�ڼ䣬ÿ���19:00-21:00����ʹ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = -1
		return bRet, bIgnore
	else
		if yueguangbaohe == 1 then
		return 0, bIgnore
		end
	end
end

function Yueliangshi_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)

role.AddRoleItem(MapID, InstanceID, RoleID, 6000005, 1, -1, 8, 420) -- �������ʯ

end


aux.RegisterItemEvent(6000010, 0, "Yueliangshi_CanUse")
aux.RegisterItemEvent(6000010, 1, "Yueliangshi_OnUse")

























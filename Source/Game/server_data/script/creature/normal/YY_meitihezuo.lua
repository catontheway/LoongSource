--1534206	С���
--1534207	��ְܰ�
--1534208	�������
--4900603	͸������1
--4900604	͸������2
--4005203	[ʥ]YYа��
--�̶���ˢ�֣���¹ (3017299663, 1765, 12413, 2465)



function YY_xiaohuanxiong_OnRespawn(MapID, InstanceID, TargetID)

		local  MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 100, 363)--��Ƥ��С���������¹[1765, 2465]�������ң����·Ӣ��ǰ��ѱ����
				msg.DispatchWorldMsgEvent(MsgID)
end
aux.RegisterCreatureEvent(1534206,1,"YY_xiaohuanxiong_OnRespawn")


function YY_xiaohuanxiong_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

		local  MsgID = msg.BeginMsgEvent()
		--##ѱ������Ƥ��С��ܣ���ŭ����ְֽܰ���5���Ӻ����
				msg.AddMsgEvent(MsgID, 100, 359)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.DispatchWorldMsgEvent(MsgID)
	    map.MapCreateCreature(3017299663, InstanceID, 4900603, 1765, 12413, 2465)
end

aux.RegisterCreatureEvent(1534206,4,"YY_xiaohuanxiong_OnDie")

function YY_tmhz_1_OnCreatureDisappear(MapID, InstanceID, TargetID, AI)
	    map.MapCreateCreature(3017299663, InstanceID, 1534207, 1765, 12413, 2465)
end

aux.RegisterCreatureEvent(4900603,13,"YY_tmhz_1_OnCreatureDisappear")


function YY_huanxiongbaba_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

		local  MsgID = msg.BeginMsgEvent()
		--##ѱ������ְ֣ܰ������������轫��5���Ӻ����
				msg.AddMsgEvent(MsgID, 100, 360)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.DispatchWorldMsgEvent(MsgID)
	    map.MapCreateCreature(3017299663, InstanceID, 4900604, 1765, 12413, 2465)
end

aux.RegisterCreatureEvent(1534207,4,"YY_huanxiongbaba_OnDie")

function YY_tmhz_2_OnCreatureDisappear(MapID, InstanceID, TargetID, AI)
	    map.MapCreateCreature(3017299663, InstanceID, 1534208, 1765, 12413, 2465)
end

aux.RegisterCreatureEvent(4900604,13,"YY_tmhz_2_OnCreatureDisappear")

function YY_huanxiongmama_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	    local a = role.GetBagFreeSize(RoleID)
	    if a < 1 then
				local  MsgID = msg.BeginMsgEvent()
		        msg.AddMsgEvent(MsgID, 100, 362) --##ѱ����������裬�����ڱ����������ѵ�һ����##�ܶ��ˣ�
		        msg.AddMsgEvent(MsgID, 2, RoleID)
		        msg.AddMsgEvent(MsgID, 4, 4005203)
		        msg.DispatchWorldMsgEvent(MsgID)
        else
				local temp = math.random(1,10)
			if temp >= 5 then
				--50%���ʻ��
		        local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 100, 361) --##������������裬���˵ĵõ�һֻ�ѵ�һ����##��
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 4, 4005203)
				msg.DispatchWorldMsgEvent(MsgID)
		        role.AddRoleItem(MapID, InstanceID, RoleID, 4005203, 1, -1, 3, 420)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 100, 365) --##������������裬�ź����ǣ�δ�ܼ�����˵�е�##��
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 4, 4005203)
				msg.DispatchWorldMsgEvent(MsgID)
		    end
		return 1
		end
end

aux.RegisterCreatureEvent(1534208,4,"YY_huanxiongmama_OnDie")



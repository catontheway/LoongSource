
--����ڻ

Guoqing_Creature = {}
--����
Guoqing_Creature[1] = { TypeID=3610224,x=1697,y=26846,z=663,CreID=0}--ͭ������	1697	663
Guoqing_Creature[2] = { TypeID=3610224,x=1705,y=26846,z=658,CreID=0}
Guoqing_Creature[3] = { TypeID=3610224,x=1688,y=26846,z=668,CreID=0}
Guoqing_Creature[4] = { TypeID=3610226,x=1717,y=26846,z=650,CreID=0}--����ǿ��	1717	650
Guoqing_Creature[5] = { TypeID=3610225,x=1678,y=26846,z=683,CreID=0}
Guoqing_Creature[6] = { TypeID=3610225,x=1677,y=26846,z=692,CreID=0}
Guoqing_Creature[7] = { TypeID=3610225,x=1677,y=26846,z=701,CreID=0}
Guoqing_Creature[8] = { TypeID=3610225,x=1677,y=26846,z=711,CreID=0}
Guoqing_Creature[9] = { TypeID=3610225,x=1678,y=26846,z=674,CreID=0}--�������	1678	674
Guoqing_Creature[10] = { TypeID=3610226,x=1751,y=26846,z=669,CreID=0}
Guoqing_Creature[11] = { TypeID=3610226,x=1742,y=26846,z=664,CreID=0}
Guoqing_Creature[12] = { TypeID=3610226,x=1734,y=26846,z=660,CreID=0}
Guoqing_Creature[13] = { TypeID=3610226,x=1725,y=26846,z=654,CreID=0}
Guoqing_Creature[14] = { TypeID=3610227,x=1711,y=26846,z=686,CreID=0}--����ħ��  1711 26846 686 ���У�
--NPC
Guoqing_Creature[15] = { TypeID=3610228,x=2106,y=18340,z=2669,CreID=0}--�Ƶ۳�����  2106 2669
Guoqing_Creature[16] = { TypeID=3610228,x=2116,y=18340,z=2669,CreID=0}
Guoqing_Creature[17] = { TypeID=3610228,x=2126,y=18340,z=2669,CreID=0}
Guoqing_Creature[18] = { TypeID=3610228,x=2096,y=18340,z=2669,CreID=0}
Guoqing_Creature[19] = { TypeID=3610228,x=2086,y=18340,z=2669,CreID=0}
Guoqing_Creature[20] = { TypeID=3610229,x=2106,y=18340,z=2677,CreID=0}--��������	2106 2677
Guoqing_Creature[21] = { TypeID=3610229,x=2116,y=18340,z=2677,CreID=0}
Guoqing_Creature[22] = { TypeID=3610229,x=2126,y=18340,z=2677,CreID=0}
Guoqing_Creature[23] = { TypeID=3610229,x=2096,y=18340,z=2677,CreID=0}
Guoqing_Creature[24] = { TypeID=3610229,x=2086,y=18340,z=2677,CreID=0}
Guoqing_Creature[25] = { TypeID=3610230,x=2106,y=18340,z=2688,CreID=0}--�������� 	2106 2688(��)		2116 2688		2126 2688	 2096 2688		2086 2688
Guoqing_Creature[26] = { TypeID=3610230,x=2116,y=18340,z=2688,CreID=0}
Guoqing_Creature[27] = { TypeID=3610230,x=2126,y=18340,z=2688,CreID=0}
Guoqing_Creature[28] = { TypeID=3610230,x=2096,y=18340,z=2688,CreID=0}
Guoqing_Creature[29] = { TypeID=3610230,x=2086,y=18340,z=2688,CreID=0}
Guoqing_Creature[30] = { TypeID=3610231,x=2105,y=18340,z=2659,CreID=0}--����  2105  18340 2659

--[[
3610224	ͭ������
3610225	����ǿ��
3610226	�������
3610227	����ħ��
3610228	�Ƶ۳�����
3610229	��������
3610230	��������
3610231	����
]]--



-- �Ϲ� 2
-- ��Ц 6
-- ���� 7
-- ���� 10
-- ���� 13

--9437201		���Ƶȴ�
--9437301		�Ϲ��ȴ�
--9437401		���񡤻���
--9437501		����ȴ�

-- ��ˮ	�������� ObjID = 6307  --
-- ��¹ �������� ObjID = 10994 --



Npcyuebing = 0		--NPC�ı�
Guaiwuyuebing = 0	--�����ı�
Tiaoxincishu = 0  --����������ҽ������ƴ���


function Guoqing_OnTimerMin(actID)

	if 	Guoqingjie == 1 then

		local curmon = tonumber(os.date("%m"))
		local curday = tonumber(os.date("%d"))
		local curhour = tonumber(os.date("%H")) -- ��ȡ��������ǰСʱ��
		local curmin = tonumber(os.date("%M"))  -- ��ȡ��������ǰ������
		--����ˮ��1715��691������ˢ�¹����ı�


		if curhour == 10 or curhour == 11 then
			if Guoqing_Creature[1].CreID == 0  then
				Guoqing_Creature[1].CreID =  map.MapCreateColCreature(3017299919, -1, Guoqing_Creature[1].TypeID, Guoqing_Creature[1].x, Guoqing_Creature[1].y, Guoqing_Creature[1].z, 1)
				Guoqing_Creature[2].CreID =  map.MapCreateColCreature(3017299919, -1, Guoqing_Creature[2].TypeID, Guoqing_Creature[2].x, Guoqing_Creature[2].y, Guoqing_Creature[2].z, 1)
				Guoqing_Creature[3].CreID =  map.MapCreateColCreature(3017299919, -1, Guoqing_Creature[3].TypeID, Guoqing_Creature[3].x, Guoqing_Creature[3].y, Guoqing_Creature[3].z, 1)
				Guoqing_Creature[4].CreID =  map.MapCreateColCreature(3017299919, -1, Guoqing_Creature[4].TypeID, Guoqing_Creature[4].x, Guoqing_Creature[4].y, Guoqing_Creature[4].z, 1)
				Guoqing_Creature[5].CreID =  map.MapCreateColCreature(3017299919, -1, Guoqing_Creature[5].TypeID, Guoqing_Creature[5].x, Guoqing_Creature[5].y, Guoqing_Creature[5].z, 1)
				Guoqing_Creature[6].CreID =  map.MapCreateColCreature(3017299919, -1, Guoqing_Creature[6].TypeID, Guoqing_Creature[6].x, Guoqing_Creature[6].y, Guoqing_Creature[6].z, 1)
				Guoqing_Creature[7].CreID =  map.MapCreateColCreature(3017299919, -1, Guoqing_Creature[7].TypeID, Guoqing_Creature[7].x, Guoqing_Creature[7].y, Guoqing_Creature[7].z, 1)
				Guoqing_Creature[8].CreID =  map.MapCreateColCreature(3017299919, -1, Guoqing_Creature[8].TypeID, Guoqing_Creature[8].x, Guoqing_Creature[8].y, Guoqing_Creature[8].z, 1)
				Guoqing_Creature[9].CreID =  map.MapCreateColCreature(3017299919, -1, Guoqing_Creature[9].TypeID, Guoqing_Creature[9].x, Guoqing_Creature[9].y, Guoqing_Creature[9].z, 1)
				Guoqing_Creature[10].CreID =  map.MapCreateColCreature(3017299919, -1, Guoqing_Creature[10].TypeID, Guoqing_Creature[10].x, Guoqing_Creature[10].y, Guoqing_Creature[10].z, 1)
				Guoqing_Creature[11].CreID =  map.MapCreateColCreature(3017299919, -1, Guoqing_Creature[11].TypeID, Guoqing_Creature[11].x, Guoqing_Creature[11].y, Guoqing_Creature[11].z, 1)
				Guoqing_Creature[12].CreID =  map.MapCreateColCreature(3017299919, -1, Guoqing_Creature[12].TypeID, Guoqing_Creature[12].x, Guoqing_Creature[12].y, Guoqing_Creature[12].z, 1)
				Guoqing_Creature[13].CreID =  map.MapCreateColCreature(3017299919, -1, Guoqing_Creature[13].TypeID, Guoqing_Creature[13].x, Guoqing_Creature[13].y, Guoqing_Creature[13].z, 1)
				Guoqing_Creature[14].CreID =  map.MapCreateColCreature(3017299919, -1, Guoqing_Creature[14].TypeID, Guoqing_Creature[14].x, Guoqing_Creature[14].y, Guoqing_Creature[14].z, 1)
				Guaiwuyuebing = 1
			end
		end

		if  curhour == 11 and curmin == 55 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 4003)  --�ٹ�5���ӣ����������ı���ʽ��Ҫ�����ˣ����λ���ץ��ʱ�䣻
			msg.DispatchWorldMsgEvent(MsgID)
		end

		if curhour == 12  and curmin == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 4004)  --���������ı���ʽ�Ѿ������ˣ���л��ҵĻ�������
			msg.DispatchWorldMsgEvent(MsgID)
			for i = 1 , 14 do
				if Guoqing_Creature[i].CreID ~= 0 then
					map.MapDeleteCreature(3017299919, -1, Guoqing_Creature[i].CreID) --ɾ������
					Guoqing_Creature[i].CreID = 0
				end
			end
			Guaiwuyuebing = 0
		end

		--�ڻƵ۳ǣ�2105,2704��ˢ������


		if curhour == 19 then
			if Guoqing_Creature[15].CreID == 0 then
				Guoqing_Creature[15].CreID =  map.MapCreateColCreature(3017299663, -1, Guoqing_Creature[15].TypeID, Guoqing_Creature[15].x, Guoqing_Creature[15].y, Guoqing_Creature[15].z, 1)
				Guoqing_Creature[16].CreID =  map.MapCreateColCreature(3017299663, -1, Guoqing_Creature[16].TypeID, Guoqing_Creature[16].x, Guoqing_Creature[16].y, Guoqing_Creature[16].z, 1)
				Guoqing_Creature[17].CreID =  map.MapCreateColCreature(3017299663, -1, Guoqing_Creature[17].TypeID, Guoqing_Creature[17].x, Guoqing_Creature[17].y, Guoqing_Creature[17].z, 1)
				Guoqing_Creature[18].CreID =  map.MapCreateColCreature(3017299663, -1, Guoqing_Creature[18].TypeID, Guoqing_Creature[18].x, Guoqing_Creature[18].y, Guoqing_Creature[18].z, 1)
				Guoqing_Creature[19].CreID =  map.MapCreateColCreature(3017299663, -1, Guoqing_Creature[19].TypeID, Guoqing_Creature[19].x, Guoqing_Creature[19].y, Guoqing_Creature[19].z, 1)
				Guoqing_Creature[20].CreID =  map.MapCreateColCreature(3017299663, -1, Guoqing_Creature[20].TypeID, Guoqing_Creature[20].x, Guoqing_Creature[20].y, Guoqing_Creature[20].z, 1)
				Guoqing_Creature[21].CreID =  map.MapCreateColCreature(3017299663, -1, Guoqing_Creature[21].TypeID, Guoqing_Creature[21].x, Guoqing_Creature[21].y, Guoqing_Creature[21].z, 1)
				Guoqing_Creature[22].CreID =  map.MapCreateColCreature(3017299663, -1, Guoqing_Creature[22].TypeID, Guoqing_Creature[22].x, Guoqing_Creature[22].y, Guoqing_Creature[22].z, 1)
				Guoqing_Creature[23].CreID =  map.MapCreateColCreature(3017299663, -1, Guoqing_Creature[23].TypeID, Guoqing_Creature[23].x, Guoqing_Creature[23].y, Guoqing_Creature[23].z, 1)
				Guoqing_Creature[24].CreID =  map.MapCreateColCreature(3017299663, -1, Guoqing_Creature[24].TypeID, Guoqing_Creature[24].x, Guoqing_Creature[24].y, Guoqing_Creature[24].z, 1)
				Guoqing_Creature[25].CreID =  map.MapCreateColCreature(3017299663, -1, Guoqing_Creature[25].TypeID, Guoqing_Creature[25].x, Guoqing_Creature[25].y, Guoqing_Creature[25].z, 1)
				Guoqing_Creature[26].CreID =  map.MapCreateColCreature(3017299663, -1, Guoqing_Creature[26].TypeID, Guoqing_Creature[26].x, Guoqing_Creature[26].y, Guoqing_Creature[26].z, 1)
				Guoqing_Creature[27].CreID =  map.MapCreateColCreature(3017299663, -1, Guoqing_Creature[27].TypeID, Guoqing_Creature[27].x, Guoqing_Creature[27].y, Guoqing_Creature[27].z, 1)
				Guoqing_Creature[28].CreID =  map.MapCreateColCreature(3017299663, -1, Guoqing_Creature[28].TypeID, Guoqing_Creature[28].x, Guoqing_Creature[28].y, Guoqing_Creature[28].z, 1)
				Guoqing_Creature[29].CreID =  map.MapCreateColCreature(3017299663, -1, Guoqing_Creature[29].TypeID, Guoqing_Creature[29].x, Guoqing_Creature[29].y, Guoqing_Creature[29].z, 1)
				Guoqing_Creature[30].CreID =  map.MapCreateColCreature(3017299663, -1, Guoqing_Creature[30].TypeID, Guoqing_Creature[30].x, Guoqing_Creature[30].y, Guoqing_Creature[30].z, 1)

				Npcyuebing = 1
			end
		end

		if  curhour == 11 and curmin == 55 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 4005)  --�ٹ�5���ӣ����������ı���ʽ��Ҫ�����ˣ����λ���ץ��ʱ�䣻
			msg.DispatchWorldMsgEvent(MsgID)
		end

		if curhour == 20 and curmin == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 4006)  --���������ı���ʽ�Ѿ������ˣ���л��ҵĲ���
			msg.DispatchWorldMsgEvent(MsgID)
			for s = 15 , 30 do
				if Guoqing_Creature[s].CreID ~= 0 then
					map.MapDeleteCreature(3017299663, -1, Guoqing_Creature[s].CreID) -- ɾ��NPC
					Guoqing_Creature[s].CreID = 0
				end
			end
			Npcyuebing = 0
		end

		if (curmon == 10 and curday == 7) and (curhour == 23 and curmin == 55) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 4002)	--������Ҫ�����ˣ�����ץ��ʱ��
			msg.DispatchWorldMsgEvent(MsgID)
		end
	end
end



function Guoqing_OnStart(actID)
	if Guoqingjie == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 4007)	--�����Ѿ���ʼ��,���λ��ҵ������ҵ��������� ����[2183,2357]����ù�������ϸ���ܣ�
		msg.DispatchWorldMsgEvent(MsgID)
	end
end

function Guoqing_OnEnd(actID)
	if Guoqingjie == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 4008)	--����ڻ�Ѿ�������,��л��ҵĻ������롣
		msg.DispatchWorldMsgEvent(MsgID)
		Guoqingjie = 0
	end
end

--ע��û������¼�
aux.RegisterActEvent(153, 4, "Guoqing_OnTimerMin")	--�ID
aux.RegisterActEvent(153, 2, "Guoqing_OnStart")	--�ID
aux.RegisterActEvent(153, 3, "Guoqing_OnEnd")		--�ID


















































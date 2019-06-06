--��Ϧ� ����
qixi_map_m04 = {}
qixi_map_m04[1]={x=1300,y=13342,z=1234}
qixi_map_m04[2]={x=1123,y=13000,z=816}
qixi_map_m04[3]={x=1626,y=14330,z=678}
qixi_map_m04[4]={x=2369,y=12733,z=974}
qixi_map_m04[5]={x=1966,y=13038,z=1463}
qixi_map_m04[6]={x=2395,y=13118,z=1509}
qixi_map_m04[7]={x=3009,y=21794,z=1808}
qixi_map_m04[8]={x=2439,y=12980,z=2128}
qixi_map_m04[9]={x=2465,y=20840,z=3055}
qixi_map_m04[10]={x=1826,y=12520,z=2778}
qixi_map_m04[11]={x=1645,y=12575,z=2308}
qixi_map_m04[12]={x=1612,y=12702,z=1922}
qixi_map_m04[13]={x=1073,y=12966,z=1927}
qixi_map_m04[14]={x=1277,y=13156,z=1492}
qixi_map_m04[15]={x=1286,y=13243,z=1241}
qixi_map_m04[16]={x=1732,y=13243,z=1197}
qixi_map_m04[17]={x=2802,y=12317,z=1114}
qixi_map_m04[18]={x=1881,y=13085,z=1662}
qixi_map_m04[19]={x=1779,y=12575,z=2364}
qixi_map_m04[20]={x=2718,y=20883,z=3350}


qixi_map_m02 = {}
qixi_map_m02[1]={x=2311,y=3879,z=1125}
qixi_map_m02[2]={x=3342,y=4635,z=1042}
qixi_map_m02[3]={x=3266,y=3924,z=853}
qixi_map_m02[4]={x=3680,y=3887,z=797}
qixi_map_m02[5]={x=3284,y=3661,z=549}
qixi_map_m02[6]={x=3676,y=3637,z=1890}
qixi_map_m02[7]={x=3511,y=3648,z=2014}
qixi_map_m02[8]={x=3127,y=4188,z=2157}
qixi_map_m02[9]={x=2985,y=3642,z=2340}
qixi_map_m02[10]={x=3100,y=6930,z=2540}
qixi_map_m02[11]={x=2779,y=6282,z=2562}
qixi_map_m02[12]={x=3359,y=5667,z=2973}
qixi_map_m02[13]={x=3130,y=6075,z=3079}
qixi_map_m02[14]={x=3408,y=6719,z=3752}
qixi_map_m02[15]={x=3125,y=7550,z=3628}
qixi_map_m02[16]={x=2710,y=6989,z=3234}
qixi_map_m02[17]={x=1725,y=8177,z=3294}
qixi_map_m02[18]={x=1472,y=8002,z=3076}
qixi_map_m02[19]={x=1015,y=7823,z=3642}
qixi_map_m02[20]={x=1699,y=3995,z=2113}


m02_qixi_point={}
m02_qixi_point[1]={mapcrc=3017298127, x=2296, y=5756, z=2204, npc=0}
m02_qixi_point[2]={mapcrc=3017298127, x=3265, y=4885, z=1597, npc=0}
m02_qixi_point[3]={mapcrc=3017298127, x=3184, y=8390, z=3356, npc=0}
m02_qixi_point[4]={mapcrc=3017298127, x=1925, y=8000, z=3624, npc=0}

m04_qixi_point={}
m04_qixi_point[1]={mapcrc=3017299663, x=2074, y=18342, z=2425, npc=0}
m04_qixi_point[2]={mapcrc=3017299663, x=1099, y=13199, z=1683, npc=0}
m04_qixi_point[3]={mapcrc=3017299663, x=3053, y=23613, z=1462, npc=0}
m04_qixi_point[4]={mapcrc=3017299663, x=2745, y=13900, z=800, npc=0}

--[[jinge1 = 0
yumei1 = 0
jinge2 = 0
yumei2 = 0]]

function QiXi_OnStart(actID)

    if act.GetActScriptData(99, 1, 42) == 0 then   --�����������
	    for i=0, 29 do
		    act.SetActScriptData(151, 1, i, 0)
		end
        act.SetActScriptData(99, 1, 42, 1)
		act.SaveActScriptData(99)
		act.SaveActScriptData(151)
	end

	local k = act.GetActScriptData(151, 1, 30)
	local curyear = tonumber(os.date("%Y"))
	if k == 0 or k == nil or k ~= curyear then
	    for i=0, 29 do
		    act.SetActScriptData(151, 1, i, 0)
		end
		act.SetActScriptData(151, 1, 30, curyear)
		act.SaveActScriptData(151)
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100138)  --��Ϧ���й��Ĵ�ͳ���˽ڣ�Ը���еĵ������˶����ճɾ�������Ϧ�����ؽ����������ǺͻƵ۳ǵĽ�紦���в�ѯ��
	msg.DispatchBroadcast(MsgID,-1,-1,-1)

	--[[jinge1 = map.MapCreateCreature(3017298127, -1, , , 6646, )
	--yumei1 = map.MapCreateCreature(3017298127, -1, , , 6646, )
	--jinge2 = map.MapCreateCreature(3017299663, -1, , , 18345, )
	--yumei2 = map.MapCreateCreature(3017299663, -1, , , 18345, )
	for randindex=1,4 do
		m02_qixi_point[randindex].npc = 0
		m04_qixi_point[randindex].npc = 0
	end

	local a = math.random(4)
	local b = math.random(4)
	local c = 4

	while (b == a and b > 0) do
		b = c
		c = c - 1
	end

	map.MapCreateColCreature(3017298127, -1, 4900492, m02_qixi_point[a].x, m02_qixi_point[a].y, m02_qixi_point[a].z, 1)
	m02_qixi_point[a].npc = 1                   -- ţ��
	map.MapCreateColCreature(3017298127, -1, 4900492, m04_qixi_point[a].x, m04_qixi_point[a].y, m04_qixi_point[a].z, 1)
	m04_qixi_point[a].npc = 1                   -- ţ��
	map.MapCreateColCreature(3017299663, -1, 4900493, m02_qixi_point[b].x, m02_qixi_point[b].y, m02_qixi_point[b].z, 1)
	m02_qixi_point[a].npc = 2                   -- ֯Ů
	map.MapCreateColCreature(3017299663, -1, 4900493, m04_qixi_point[b].x, m04_qixi_point[b].y, m04_qixi_point[b].z, 1)
	m04_qixi_point[a].npc = 2                   -- ֯Ů

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, )  --�·�ţ�ɺ��·�֯Ů�Ѿ������������Ǻ���¹��������ȥ�����Ƕһ���Ե�ᡣ
	msg.DispatchWorldMsgEvent(MsgID)

	for randindex=1,20 do
		map.MapCreateColCreature(3017298127, -1, 1534144, qixi_map_m04[randindex].x, qixi_map_m04[randindex].y, qixi_map_m04[randindex].z, 1)
		map.MapCreateColCreature(3017299663, -1, 1534145, qixi_map_m02[randindex].x, qixi_map_m02[randindex].y, qixi_map_m02[randindex].z, 1)
	end

	if curmin == 30 or curmin == 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, )  --��Ϧ��ڼ�ÿ10���Ӷ���ˢ����ּ�������ɱ����Ի�÷�ţ���ͽ�����
		msg.DispatchWorldMsgEvent(MsgID)
	end]]

end

function QiXi_OnTimerMin(actID)

	local curhour = tonumber(os.date("%H"))
	local curmin = tonumber(os.date("%M"))

	if (curhour>= 19  and curhour<= 22 ) and curmin % 10==0 then

	    for randindex=1,4 do
            m02_qixi_point[randindex].npc = 0
			m04_qixi_point[randindex].npc = 0
		end

		local a = math.random(4)
		local b = math.random(4)
		local c = 4

		while (b == a and b > 0) do
		    b = c
			c = c - 1
			a = c
		end

		map.MapCreateColCreature(3017298127, -1, 4900492, m02_qixi_point[a].x, m02_qixi_point[a].y, m02_qixi_point[a].z, 1)
		m02_qixi_point[a].npc = 1                   -- ţ��
		map.MapCreateColCreature(3017299663, -1, 4900492, m04_qixi_point[a].x, m04_qixi_point[a].y, m04_qixi_point[a].z, 1)
		m04_qixi_point[a].npc = 1                   -- ţ��
		map.MapCreateColCreature(3017298127, -1, 4900493, m02_qixi_point[b].x, m02_qixi_point[b].y, m02_qixi_point[b].z, 1)
		m02_qixi_point[b].npc = 2                   -- ֯Ů
		map.MapCreateColCreature(3017299663, -1, 4900493, m04_qixi_point[b].x, m04_qixi_point[b].y, m04_qixi_point[b].z, 1)
		m04_qixi_point[b].npc = 2                   -- ֯Ů

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 1)
		msg.AddMsgEvent(MsgID, 1, 12101)  --�·�ţ�ɺ��·�֯Ů�Ѿ������������Ǻ���¹��������ȥ�����Ƕһ���Ե�ᡣ
		msg.DispatchWorldMsgEvent(MsgID)

		for randindex=1,20 do
            map.MapCreateColCreature(3017299663, -1, 1534144, qixi_map_m04[randindex].x, qixi_map_m04[randindex].y, qixi_map_m04[randindex].z, 1)
		    map.MapCreateColCreature(3017298127, -1, 1534145, qixi_map_m02[randindex].x, qixi_map_m02[randindex].y, qixi_map_m02[randindex].z, 1)
        end

		if curmin == 30 or curmin == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 12102)  --��Ϧ��ڼ�ÿ10���Ӷ���ˢ����ּ�������ɱ����Ի�÷�ţ���ͽ�����
			msg.DispatchWorldMsgEvent(MsgID)
	    end

	end

	if curhour == 23 and curmin == 0 then
		m02_qixi_point[1].npc = 0
		m02_qixi_point[2].npc = 0
		m02_qixi_point[3].npc = 0
		m02_qixi_point[4].npc = 0
		m04_qixi_point[1].npc = 0
		m04_qixi_point[2].npc = 0
		m04_qixi_point[3].npc = 0
		m04_qixi_point[4].npc = 0
	end

end

--[[npc����ʱ�䵽����������ڸ�������ɾ����npc
function QiXi_OnEnd(actID)
	map.MapDeleteCreature(3017298127, -1, jinge1)
	map.MapDeleteCreature(3017298127, -1, yumei1)
	map.MapDeleteCreature(3017299663, -1, jinge2)
	map.MapDeleteCreature(3017299663, -1, yumei2)
end]]

--ע��û������¼�
aux.RegisterActEvent(151, 2, "QiXi_OnStart")
--aux.RegisterActEvent(, 3, "QiXi_OnEnd")
aux.RegisterActEvent(151, 4, "QiXi_OnTimerMin")

--����Ի�
function n4900490_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    if TalkIndex == -1 then
	    local bStart = act.GetActIsStart(151)
	    if bStart then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 12103) -- "��磺\n        ����ǣţ�ǣ���Ӻ�Ů��\n        ����ߪ���֣�����Ū���̡�\n      ���ղ����£����������ꡣ\n        �Ӻ�����ǳ����ȥ������\n        ӯӯһˮ�䣬���������"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, 5) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 12104) -- ���һ���Ե�����а�ǰʮ������
			msg.AddMsgEvent(MsgID, 21, 4)   --ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 12105)   --"��Ϧ������"
			msg.AddMsgEvent(MsgID, 21, 6)   --ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 12106)   --"��Ϧ�˵��"
			msg.AddMsgEvent(MsgID, 21, 7)   --ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 12107)   --"��Ϧ������ս���"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 12103) -- "��磺\n        ����ǣţ�ǣ���Ӻ�Ů��\n        ����ߪ���֣�����Ū���̡�\n      ���ղ����£����������ꡣ\n        �Ӻ�����ǳ����ȥ������\n        ӯӯһˮ�䣬���������"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, 5) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 12104) -- ���һ���Ե�����а�ǰʮ������
			msg.AddMsgEvent(MsgID, 21, 8)   --��ȡ����
			msg.AddMsgEvent(MsgID, 1, 12108)   --"��ȡ���ս���"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
    elseif TalkIndex == 4 then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 12109) -- "��磺\n        ���ְָ�����˵������ǰֻ��һ������ͨͨ��ũ�ҷ�ţ�ޣ��󲮺����������ܲ��á�������һ�죬������Ϧ�����ϻ�ţ��Ȼ����˵���ˣ������������������죬���ϵ���Ů�����·�Ϸˮ��ֻҪ�ܰ���Ů������͵͵�Ĳ�����������������Ů��Ϊ�Լ������ӡ��ְ��ջ�ţ�Ļ����ˣ����豾���������������ǿ����ְ��Ǻ����Ц��������������������Ϊ���������ӡ���ôһ�����������Һ����ã�������ĸ�����������˽���˵������ǳ��������������������ץ�ߣ����÷��������ϻ���һ�����ӣ����������һ�����������·�����涨ֻ��ÿ����������գ����ǲ�����ȵ�������һ�Ρ���"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	elseif TalkIndex == 6 then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 12110) -- "��磺\n        ��Ϧ�Ļʱ���ǣ�8��16����8��30�ա�\n        ��Ϧ�Ļ���ݣ���Ϧ��ڼ䣬ÿ������19����23�㣬ÿ��10���Ӿͻ������Ǻ���¹�ĳ����ڳ����·�ţ�ɡ��·�֯Ů���߷�ּ�������ţ�ɴ������ý��������һ���Ե�ᣬ��֯Ů�������÷�ţ���һ���Ե�ᡣ��ּ�������Ա����������������Ի�ý����󡢷�ţ�������������ߡ���ţ�ɺ�֯Ů���ʹ�ý���������ӵ�����͡����ǡ������Ի��һö������Ϊ��ö����������߷�ţ�������⣬������ͷ�ţ���������ڻ�ڼ��ڸ�������BOSS���ϻ�á�\n        �����������ô�ʹ����Ե��һ�������Ʒ������"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
    elseif TalkIndex == 7 then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 12111) -- "��磺\n        ��ѡ�������ƺţ���Լȵ�ţ�������ĸ���Ρ�\n        �һ���Ե��������ĵ�1-3����ҿ��Ի�����������ߣ���֮���7�����ֻ��ѡȡ���е�1����"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
    elseif TalkIndex == 5 then
	    local k = nil
		for i = 0, 9 do
		    if act.GetActScriptData(151, 1, i) ~= nil and act.GetActScriptData(151, 1, i) > 0 then
			    k = i
			end
		end
		if k == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 12112) -- "��磺\n        ÿ�������ߵ㵽ʮһ�㣬���������·�ţ�ɺ��·�֯Ů���һ���Ե�ᡣ���ڻ�û���˶һ�����Ե�ᣡ"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 12113+k) -- "��磺\n        ���ڶһ���Ե�����������ǣ�\n        XXX        XXX����Ե��
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			for i = 0,k do
				msg.AddMsgEvent(MsgID, 9, act.GetActScriptData(151, 1, i+10))
				msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(151, 1, i))
			end
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
	elseif TalkIndex == 8 then
	    local k = nil
		for i = 0, 9 do
		    if act.GetActScriptData(151, 1, i) ~= nil and act.GetActScriptData(151, 1, i) > 0 and act.GetActScriptData(151, 1, i) == RoleID then
			    k = i
				break
			end
		end
		if k == nil or act.GetActIsStart(151) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 12127) -- "��磺\n        �ܱ�Ǹ����û�н������а�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif act.GetActScriptData(151, 1, k+20) == 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 12128) -- "��磺\n        �Բ������Ѿ���ȡ�������ˡ�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
		    if k >= 0 and k <= 2 then
			    if role.GetBagFreeSize(RoleID) > 1 then
				    local attval1=role.GetRoleAttValue(MapID, InstanceID, RoleID, 1)
					local attval2=role.GetRoleAttValue(MapID, InstanceID, RoleID, 3)
					if attval1 > attval2 then
						role.AddRoleItem(MapID, InstanceID, RoleID, 8600099, 1, 5, 13, 1001)  --����
					else
						role.AddRoleItem(MapID, InstanceID, RoleID, 8600098, 1, 5, 13, 1001)  --����
					end
					role.AddRoleItem(MapID, InstanceID, RoleID, 3305007, 1, -1, 13, 1001)  --�ƺ�
					act.SetActScriptData(151, 1, k+20, 1)
					act.SaveActScriptData(151)
				else
				    local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 12123) -- "��磺\n        ������֤�Լ������������㹻�Ŀռ�������ȡ��"
					msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
					msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
				end
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 12124) -- "��磺\n        ��ѡ������Ҫ��ȡ�Ľ�����"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, 9) -- ��ȡ��������ĸ����
				msg.AddMsgEvent(MsgID, 1, 12125) -- ��ȷ����
				msg.AddMsgEvent(MsgID, 21, 10) -- ��ȡ�ƺţ���Լȵ��
				msg.AddMsgEvent(MsgID, 1, 12126) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			end
		end
	elseif TalkIndex == 9 then
	    local k = nil
		for i = 0, 9 do
		    if act.GetActScriptData(151, 1, i) ~= nil and act.GetActScriptData(151, 1, i) > 0 and act.GetActScriptData(151, 1, i) == RoleID then
			    k = i
				break
			end
		end
		if k == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 12127) -- "��磺\n        �ܱ�Ǹ����û�н������а�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif act.GetActScriptData(151, 1, k+20) == 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 12128) -- "��磺\n        �Բ������Ѿ���ȡ�������ˡ�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
			if role.GetBagFreeSize(RoleID) > 0 then
			    local attval1=role.GetRoleAttValue(MapID, InstanceID, RoleID, 1)
				local attval2=role.GetRoleAttValue(MapID, InstanceID, RoleID, 3)
				if attval1 > attval2 then
				    role.AddRoleItem(MapID, InstanceID, RoleID, 8600099, 1, 5, 13, 1001)  --����
				else
				    role.AddRoleItem(MapID, InstanceID, RoleID, 8600098, 1, 5, 13, 1001)  --����
				end
				act.SetActScriptData(151, 1, k+20, 1)
				act.SaveActScriptData(151)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 12129) -- "��磺\n        ������֤�Լ������������㹻�Ŀռ�������ȡ��"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			end
		end
	elseif TalkIndex == 10 then
	    local k = nil
		for i = 0, 9 do
		    if act.GetActScriptData(151, 1, i) ~= nil and act.GetActScriptData(151, 1, i) > 0 and act.GetActScriptData(151, 1, i) == RoleID then
			    k = i
				break
			end
		end
		if k == nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 12127) -- "��磺\n        �ܱ�Ǹ����û�н������а�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif act.GetActScriptData(151, 1, k+20) == 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 12128) -- "��磺\n        �Բ������Ѿ���ȡ�������ˡ�"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
			if role.GetBagFreeSize(RoleID) > 0 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 3305007, 1, -1, 13, 1001)  --�ƺ�
				act.SetActScriptData(151, 1, k+20, 1)
				act.SaveActScriptData(151)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 12129) -- "��磺\n        ������֤�Լ������������㹻�Ŀռ�������ȡ��"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			end
		end
    end
end

aux.RegisterCreatureEvent(4900490, 7, "n4900490_OnTalk")

function C4900492_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    if -1 == TalkIndex then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 12130) -- "�·�ţ�ɣ�\n        ���������������ý�����һ���Ե�ᡣ\n        ���ĵظ���˵һ�������������������Χʹ�����ǵĽ��������Ļ�������õ��ҵ�ף�����õ�һ����ţ����"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, 4) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 21) -- ���һ���Ե�ᡱ
		msg.AddMsgEvent(MsgID, 21, 5) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 22) -- ��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	elseif 4 == TalkIndex then
	    local i = role.GetRoleItemNum(RoleID, 3305002)
		if i < 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 12131) -- "�·�ţ�ɣ�\n        �һ���Ե����Ҫһ������������׼���ú�������"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif role.GetBagFreeSize(RoleID) == 0 then
			    msg.AddMsgEvent(MsgID, 20, 12132) -- "�·�ţ�ɣ�\n        ������֤�Լ������������㹻�Ŀռ������һ���"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
		    role.RemoveFromRole(MapID, InstanceID, RoleID, 3305002, 1, 420)
            role.AddRoleItem(MapID, InstanceID, RoleID, 3305003, 1, -1, 8, 420)
			HuoDeYinYuanJie(RoleID)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 12133) -- "�·�ţ�ɣ�\n        ����Ҫ�����һ���"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, 4) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 21) -- ���һ���Ե�ᡱ
			msg.AddMsgEvent(MsgID, 21, 5) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 22) -- ��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
	end
end

--ע��һ������Ի��¼�
aux.RegisterCreatureEvent(4900492, 7, "C4900492_OnTalk")

function C4900493_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    if -1 == TalkIndex then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 12134) -- "�·�֯Ů��\n        ���������������÷�ţ���һ���Ե�ᡣ\n        ���ĵظ���˵һ�������������������Χʹ�����ǵĽ��������Ļ�������õ��ҵ�ף�����õ�һ����ţ����"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
		msg.AddMsgEvent(MsgID, 21, 4) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 21) -- ���һ���Ե�ᡱ
		msg.AddMsgEvent(MsgID, 21, 5) -- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 22) -- ��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	elseif 4 == TalkIndex then
	    local i = role.GetRoleItemNum(RoleID, 3305001)
		if i < 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 12135) -- "�·�֯Ů��\n        �һ���Ե����Ҫһ����ţ��������׼���ú�������"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		elseif role.GetBagFreeSize(RoleID) == 0 then
			    msg.AddMsgEvent(MsgID, 20, 12136) -- "�·�֯Ů��\n        ������֤�Լ������������㹻�Ŀռ������һ���"
				msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
				msg.AddMsgEvent(MsgID, 21, -1) -- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 21) -- ��ȷ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
		    role.RemoveFromRole(MapID, InstanceID, RoleID, 3305001, 1, 420)
            role.AddRoleItem(MapID, InstanceID, RoleID, 3305003, 1, -1, 8, 420)
			HuoDeYinYuanJie(RoleID)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 12137) -- "�·�֯Ů��\n        ����Ҫ�����һ���"
			msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
			msg.AddMsgEvent(MsgID, 21, 4) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 21) -- ���һ���Ե�ᡱ
			msg.AddMsgEvent(MsgID, 21, 5) -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 22) -- ��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
	end
end

--ע��һ������Ի��¼�
aux.RegisterCreatureEvent(4900493, 7, "C4900493_OnTalk")


--��Ϧ��ˮ���
function I3305005_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 3303308, 3, -1, 8, 420)
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local k = math.floor(rolelevel/5 + 1)
	if k > 23 then
	    k = 23
	end
	if k <= 23 then
        role.AddRoleExp(MapID, InstanceID, RoleID, SanBa_exp[k]*3)
	end
	local d = math.random(4)
	role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 3303048+d, 1, -1, 8, 420)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 12138)   --���յ���������XXX����Ʒ����XXXX��ˮ��
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.AddMsgEvent(MsgID, 4, 3303048+d)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)     --������Ϣ
end

function I3305005_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)

	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	local sex = role.GetRoleSex(TargetID)
	if(sex == 1) then

		return 31, bIgnore
	end
	if(FreeSize < 2) then
		--��ʾ��ұ����ռ䲻��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2402)   --Ŀ��������ҵĿռ䲻��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		bRet = 31
		return bRet, bIgnore
	end
end

aux.RegisterItemEvent(3305005, 1, "I3305005_OnUse")
aux.RegisterItemEvent(3305005, 0, "I3305005_CanUse")


--��Ϧʱװ���
function I3305006_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local k = math.floor(rolelevel/5 + 1)
	if k > 23 then
	    k = 23
	end
	if k <= 23 then
        role.AddRoleExp(MapID, InstanceID, RoleID, SanBa_exp[k]*3)
	end
	role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 3305004, 1, -1, 8, 420)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 12138)   --���յ���������XXX����Ʒ����XXXX��ˮ��
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.AddMsgEvent(MsgID, 4, 3305004)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)     --������Ϣ
end

function I3305006_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	local sex = role.GetRoleSex(TargetID)
	if(sex == 0) then

		return 31, bIgnore
	end
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2402)   --Ŀ��������ҵĿռ䲻��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		bRet = 31
		return bRet, bIgnore
	end
end

aux.RegisterItemEvent(3305006, 1, "I3305006_OnUse")
aux.RegisterItemEvent(3305006, 0, "I3305006_CanUse")

--ʱװ�����
function I3305004_OnUse(MapID, InstanceID, TypeID, RoleID)
    local k = math.random(36)
	role.AddRoleItem(MapID, InstanceID, RoleID, 9141048+k, 1, -1, 8, 420)
end

function I3305004_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(RoleID)

	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2402)   --Ŀ��������ҵĿռ䲻��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		bRet = 32
		return bRet, bIgnore
	end
end

aux.RegisterItemEvent(3305004, 1, "I3305004_OnUse")
aux.RegisterItemEvent(3305004, 0, "I3305004_CanUse")
